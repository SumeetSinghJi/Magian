#include <windows.h>
#include <conio.h>
#include <unistd.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <filesystem>
using namespace std;
// CLASSES
class Obstacle_class
{
  public:
    char obstacle_symbol;
    string obstacle_name = "";
    string obstacle_description = "";
    int obstacle_hp = 0;
    int obstacle_x_pos;
    int obstacle_y_pos;
};
class rock_obstacle_subclass : public Obstacle_class
{
public:
  rock_obstacle_subclass()
  {
    obstacle_symbol = 'O';
    obstacle_name = "Rock";
    obstacle_description = "A strong rock that blocks line of sight but can be destroyed or climbed";
    obstacle_hp = 3;
    obstacle_x_pos = rand() % width;
    obstacle_y_pos = rand() % height;
  }
};
class tree_obstacle_subclass : public Obstacle_class
{
public:
  tree_obstacle_subclass()
  {
    obstacle_symbol = 'T';
    obstacle_name = "Tree";
    obstacle_description = "A large tree that blocks line of sight. Can be burnt down.";
    obstacle_hp = 1;
    obstacle_x_pos = rand() % width;
    obstacle_y_pos = rand() % height;
  }
};
fstream savefile_object;
enum edirection 
{
  STOP = 0, UP, DOWN, LEFT, RIGHT
};
edirection direction;
// GLOBAL POINTERS
vector<shared_ptr<Obstacle_class>> obstacles_vector;
string version = "0.2.2";
bool music_variable = true; 
bool gameover = false;
// level 1 map
const int width = 20;
const int height = 20;
char buffer[height][width];
// level 2 map
const int l2width = 40;
const int l2height = 40;
int map_size = 0; // 1 = small, 2, medium, 3, large, 4, extra large, 5 giant, 6 world map
int max_obstacle_objects = 0;
char l2buffer[l2height][l2width];
int moneyx, moneyy; // money draw() position
int score = 0;
int money = 0;
int x_pos, y_pos; // player draw() position
int lives = 3;
int difficulty=3;
int language=1; // language 1 = english
int player_speed=1;
string name = "";
int level=11;
int level_select_variable=1; // for bonus level select
clock_t lastShootTime; // shoot() time management variables
const int shootInterval = 1000; // 1 second in milliseconds
void random_generate_obstacle(int map_size, int& max_obstacle_objects, vector<shared_ptr<Obstacle_class>>& obstacles_vector)
{
  if(map_size==1) // small
  {
    max_obstacle_objects += 5;
    max_obstacle_objects += rand() % 5;
  }
  else if(map_size==2) // medium
  {
    max_obstacle_objects += 10;
    max_obstacle_objects += rand() % 5;
  }
  else if(map_size==3) // large
  {
    max_obstacle_objects += 15;
    max_obstacle_objects += rand() % 5;
  }
  else if(map_size==4) // extra-large
  {
    max_obstacle_objects += 20;
    max_obstacle_objects += rand() % 5;
  }
  else if(map_size==5) // giant
  {
    max_obstacle_objects += 25;
    max_obstacle_objects += rand() % 5;
  }
  for (int i = 0; i < max_obstacle_objects; i++)
  {
    int obstacleType = rand() % 2; // 0 for rock, 1 for tree
    if (obstacleType == 0)
    {
      shared_ptr<rock_obstacle_subclass> rock_obstacle = make_shared<rock_obstacle_subclass>();
      obstacles_vector.push_back(rock_obstacle);
    }
    else
    {
      shared_ptr<tree_obstacle_subclass> tree_obstacle = make_shared<tree_obstacle_subclass>();
      obstacles_vector.push_back(tree_obstacle);
    }
  }
}
void setup() 
{
  if (music_variable == false)
  {
    PlaySoundW(NULL, NULL, 0);
  }
  else
  {
    PlaySoundW(L"sound//music//alien-jungle.wav", NULL, SND_FILENAME | SND_ASYNC);
  }
  map_size=1;
  // reset the level variables
  score=0;
  gameover = false;

  // Below srand method needs to be executed at runtime hence run in a function vs global variable;
  moneyx = rand() % width;
  moneyy = rand() % height;

  // setting position of player
  direction = STOP;
  x_pos = width / 2;
  y_pos = height / 2;

  // setup obstacles
  // generate a random number of obstacles
  //initialise buffer with default character ' ' (space) to avoid console buffer not clearing.
  for (int i = 0; i < height; i++)
  {  
    for (int j = 0; j < width; j++)
    {
        buffer[i][j] = ' ';
    }
  }
}
void draw_level_1() 
{
  // Draw top wall  
  for (int top_wall = 0; top_wall < width; top_wall++) {
    buffer[0][top_wall] = '#';
  }

  // draw middle section
  //loop through y axis 19 times down
  for (int y = 1; y < height-1; y++) 
  {
    // loop through x axis 19 times across
    for (int x = 0; x < width; x++) 
    {
      // draw side wall
      if (x == 0 || x == width - 1) 
      {
        buffer[y][x] = '#';
      }
      //draw player
      else if (x == x_pos && y == y_pos) 
      {
        buffer[y][x] = 'P';
      }
      //draw money
      else if (x == moneyx && y == moneyy) 
      {
        buffer[y][x] = '$';
      }
      else
      {
        buffer[y][x] = ' '; // Initialize to empty space

        // Check if an obstacle or enemy exists at the current position
        bool obstacleOrEnemyFound = false;
        
        // Check obstacles
        for (const auto& obstacle : obstacles_vector)
        {
          if (x == obstacle->obstacle_x_pos && y == obstacle->obstacle_y_pos)
          {
            buffer[y][x] = obstacle->obstacle_symbol;
            obstacleOrEnemyFound = true;
            break;
          }
        }
        
        // Check enemies if no obstacle is found
        if (!obstacleOrEnemyFound)
        {
          for (const auto& enemy : enemies_vector)
          {
            if (enemy->alive && x == enemy->enemy_x_pos && y == enemy->enemy_y_pos)
            {
              buffer[y][x] = enemy->enemy_symbol;
              obstacleOrEnemyFound = true;
              break;
            }
          }
        }
      }
    }
  }

  // Draw bottom wall
  for (int bottom_wall = 0; bottom_wall < width; bottom_wall++) {
    buffer[height-1][bottom_wall] = '#';
  }

  system("cls");
  for (int y = 0; y < height; y++) 
  {
    for (int x = 0; x < width; x++) {
      cout << buffer[y][x];
    }
    cout << endl;
  }
  // print the current score and lives beneath the array
  string score_str = "Current Score: " + to_string(score);
  string lives_str = "Current Lives: " + to_string(lives);
  cout << score_str << endl;
  cout << lives_str << endl;
}
int main()
{
    return 0;
}