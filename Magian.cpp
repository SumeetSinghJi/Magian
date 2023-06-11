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
#include <chrono>
#include <algorithm>
#include "headers/save_game.h"
#include "headers/game_settings.h"
#include "headers/get_objective.h"
#include "headers/get_items.h"
#include "headers/get_skills.h"
using namespace std;
// CLASSES
class player_class
{
  public:
    string player_name = "";
    int player_magic = 0;
    int player_health = 0;
    int player_literacy = 0;
    int player_diplomacy = 0;
    int player_swimming = 0;
    int player_herbology = 0;
    int player_speed = 0;
    int player_xp = 0;
    int player_x_pos; 
    int player_y_pos;
    int previous_x_pos;
    int previous_y_pos;
    int player_money = 0;
};
// Forward declaration
class obstacle_class;
enum edirection {STOP = 0, UP, DOWN, LEFT, RIGHT};
edirection direction;
// Global variables
// level 1 map
const int width = 20;
const int height = 20;
char buffer[height][width];
// level 2 map
const int l2width = 40;
const int l2height = 40;
char l2buffer[l2height][l2width];
int map_size = 0; // 1 = small, 2, medium, 3, large, 4, extra large, 5 giant, 6 world map
// Classes
class obstacle_class
{
  public:
    char obstacle_symbol;
    string obstacle_name = "";
    string obstacle_description = "";
    int obstacle_hp = 0;
    int obstacle_x_pos;
    int obstacle_y_pos;
    bool obstacle_alive;
    void obstacle_check_collision(shared_ptr<player_class> &player_pointer_object) 
    {
        if (obstacle_x_pos == player_pointer_object->player_x_pos && obstacle_y_pos == player_pointer_object->player_y_pos) {
            cout << "You ran into the obstacle";
            cin.get();
        }
    }
};
class rock_obstacle_subclass : public obstacle_class
{
public:
    rock_obstacle_subclass()
    {
        obstacle_symbol = 'R';
        obstacle_name = "Rock";
        obstacle_description = "A strong rock that blocks line of sight but can be destroyed or climbed";
        obstacle_hp = 1;
        obstacle_x_pos = rand() % width;
        obstacle_y_pos = rand() % height;
        bool obstacle_alive = true;
    }
};
class tree_obstacle_subclass : public obstacle_class
{
public:
    tree_obstacle_subclass()
    {
        obstacle_symbol = 'T';
        obstacle_name = "Tree";
        obstacle_description = "A large tree that blocks line of sight. Can be burnt down";
        obstacle_hp = 1;
        obstacle_x_pos = rand() % width;
        obstacle_y_pos = rand() % height;
        bool obstacle_alive = true;
    }
};
class enemy_class 
{
public:
    string enemy_name;
    string enemy_description;
    int damage;
    int enemy_hp;
    int speed;
    int enemy_xp;
    int enemy_x_pos;
    int enemy_y_pos;
    int enemy_melee_damage;
    bool enemy_alive;
    int enemy_pause;
    char enemy_symbol;
    // Add random movement logic as part of the enemy_class
    void random_slow_movement() 
    {
        if (enemy_alive && enemy_pause == 0) {
            int random_direction = rand() % 4;
            switch (random_direction) {
            case 0: // Up
                if (enemy_y_pos > 1 && buffer[enemy_y_pos - 1][enemy_x_pos] != '#') {
                    enemy_y_pos--;
                }
                break;
            case 1: // Down
                if (enemy_y_pos < height - 2 && buffer[enemy_y_pos + 1][enemy_x_pos] != '#') {
                    enemy_y_pos++;
                }
                break;
            case 2: // Left
                if (enemy_x_pos > 1 && buffer[enemy_y_pos][enemy_x_pos - 1] != '#') {
                    enemy_x_pos--;
                }
                break;
            case 3: // Right
                if (enemy_x_pos < width - 2 && buffer[enemy_y_pos][enemy_x_pos + 1] != '#') {
                    enemy_x_pos++;
                }
                break;
            }
        } else if (enemy_pause > 0) {
            enemy_pause--;
        }
    }
    void random_fast_movement() 
    {
        if (enemy_alive && enemy_pause == 0) 
        {
            // Generate a random number between 0 and 9
            int random_num = rand() % 10;
            // Move the enemy only if the random number is less than 3 (30% chance)
            if (random_num < 3) 
            {
                int random_direction = rand() % 4;
                switch (random_direction) 
                {
                    case 0: // Up
                        if (enemy_y_pos > 1 && buffer[enemy_y_pos - 1][enemy_x_pos] != '#') 
                        {
                            enemy_y_pos--;
                            enemy_y_pos--;
                        }
                        break;
                    case 1: // Down
                        if (enemy_y_pos < height - 2 && buffer[enemy_y_pos + 1][enemy_x_pos] != '#') 
                        {
                            enemy_y_pos++;
                            enemy_y_pos++;
                        }
                        break;
                    case 2: // Left
                        if (enemy_x_pos > 1 && buffer[enemy_y_pos][enemy_x_pos - 1] != '#') 
                        {
                            enemy_x_pos--;
                            enemy_x_pos--;
                        }
                        break;
                    case 3: // Right
                        if (enemy_x_pos < width - 2 && buffer[enemy_y_pos][enemy_x_pos + 1] != '#') 
                        {
                            enemy_x_pos++;
                            enemy_x_pos++;
                        }
                        break;
                }
            }
        } 
        else if (enemy_pause > 0) 
        {
            enemy_pause--;
        }
    }
    void random_slow_chasing(shared_ptr<player_class> &player_pointer_object) 
    {
      // Move the enemy only if the random number is less than 4 (40% chance)
      if (enemy_alive && enemy_pause == 0)
      {
        // Generate a random number between 0 and 9
        int random_num = rand() % 10;
        if (rand() % 10 < 4) 
        {
            // Move towards player
            if (enemy_x_pos < player_pointer_object->player_x_pos && enemy_x_pos < width - 2)
                enemy_x_pos++;
            else if (enemy_x_pos > player_pointer_object->player_x_pos && enemy_x_pos > 1)
                enemy_x_pos--;
            if (enemy_y_pos < player_pointer_object->player_y_pos && enemy_y_pos < height - 2)
                enemy_y_pos++;
            else if (enemy_y_pos > player_pointer_object->player_y_pos && enemy_y_pos > 1)
                enemy_y_pos--;
        }
      }
      else if (enemy_pause > 0) 
      {
        enemy_pause--;
      }
    }
    void l2random_slow_chasing(shared_ptr<player_class> &player_pointer_object) 
    {
      // Move the enemy only if the random number is less than 4 (40% chance)
      if (enemy_alive && enemy_pause == 0)
      {
        // Generate a random number between 0 and 9
        int random_num = rand() % 10;
        if (rand() % 10 < 4) 
        {
            // Move towards player
            if (enemy_x_pos < player_pointer_object->player_x_pos && enemy_x_pos < l2width - 2)
                enemy_x_pos++;
            else if (enemy_x_pos > player_pointer_object->player_x_pos && enemy_x_pos > 1)
                enemy_x_pos--;
            if (enemy_y_pos < player_pointer_object->player_y_pos && enemy_y_pos < l2height - 2)
                enemy_y_pos++;
            else if (enemy_y_pos > player_pointer_object->player_y_pos && enemy_y_pos > 1)
                enemy_y_pos--;
        }
      }
      else if (enemy_pause > 0) 
      {
        enemy_pause--;
      }
    }
    void enemy_check_collision(shared_ptr<player_class> &player_pointer_object) 
    {
        if (enemy_x_pos == player_pointer_object->player_x_pos && enemy_y_pos == player_pointer_object->player_y_pos) 
        {
          direction = STOP;
          player_pointer_object->player_x_pos = player_pointer_object->previous_x_pos;
          player_pointer_object->player_y_pos = player_pointer_object->previous_y_pos;
            lives-=enemy_melee_damage;
            enemy_pause = 3; // Pause for 3 ticks
            cout << "You bumped into the monster";
            cin.get();
        }
    }
};
class fire_enemy_subclass : public enemy_class
{
  public:
    fire_enemy_subclass()
    {
      enemy_name="Fire";
      enemy_hp = 1;
      enemy_xp = 1;
      enemy_melee_damage = 1;
      enemy_description="A large moving flame 2 meters high burning everything it touches.";
      enemy_x_pos = rand() % width;
      enemy_y_pos = rand() % height;
      enemy_alive = true;
      enemy_pause = 0;
      enemy_symbol = 'F';
    }
};
class flying_enemy_subclass : public enemy_class
{
  public:
    flying_enemy_subclass()
    {
      enemy_name="Flying Rakashaa";
      enemy_hp = 3;
      enemy_xp = 2;
      enemy_melee_damage = 2;
      enemy_description="A flying demon with powerfull magic.";
      enemy_x_pos = rand() % width;
      enemy_y_pos = rand() % height;
      enemy_alive = true;
      enemy_pause = 0;
      enemy_symbol = '^';
    }
};
class stalker_enemy_subclass : public enemy_class
{
  public:
    stalker_enemy_subclass()
    {
      enemy_name="Stalking Rakashaa";
      enemy_hp = 3;
      enemy_xp = 1;
      enemy_melee_damage = 3;
      enemy_description="A white large furry humanoid with sharp nails, bare arms and legs despite a furry body"
                        "It's legs move exceedingly fast but stride is slow giving it the impression at any moment"
                        "it could outrace and catch you."
                        "The uncertanty of the humanoids actions cause you deep fear.";
      if(map_size==1)
      {
        enemy_x_pos = rand() % width;
        enemy_y_pos = rand() % height;
      }
      if(map_size==2)
      {
        enemy_x_pos = rand() % l2width;
        enemy_y_pos = rand() % l2height;
      }
      enemy_alive = true;
      enemy_pause = 0;
      enemy_symbol = '&';
    }
};
class item_class 
{
public:
  string item_name;
  string description;
  int cost;
  int effect;
  int item_x_pos;
  int item_y_pos;
  char item_symbol;
  bool item_alive;
  item_class(string item_name, string description, char item_symbol)
  {
    this->item_name = item_name;
    this->description = description;
    this->item_symbol = item_symbol;
    if(map_size==1)
      {
        item_x_pos = rand() % width;
        item_y_pos = rand() % height;
      }
      if(map_size==2)
      {
        item_x_pos = rand() % l2width;
        item_y_pos = rand() % l2height;
      }
  }
  virtual void use(int& value)
  {

  }
 void item_store()
  {
    // Store the item name in the savegame file
    savefile_object.open("magian_save.txt", ios::app);
    if (savefile_object.is_open())
    {
        savefile_object << item_name << endl;
        savefile_object.close();
    }
    else
    {
        cerr << "Error: Inventory not saved to save file" << endl;
        return;
    }
  }
  
  void item_check_collision(shared_ptr<player_class>& player_pointer_object) 
  {
    if (item_x_pos == player_pointer_object->player_x_pos && item_y_pos == player_pointer_object->player_y_pos)
    {
        item_alive = false;
        item_store(); // Store the item name in the save file
        cout << "You picked up the item" << endl;
        cin.get();
    }
  }
};
class potion_item_subclass : public item_class
{
public:
  potion_item_subclass() : item_class("Potion", "When used increases players life by 1 by regenerating their body.", 'p') {}
  void use(shared_ptr<player_class>& player_pointer_object)
  {
    cout << "You used a Potion. 1 life gained" << endl;
    player_pointer_object->player_health++;
  }
};
class leather_boots_item_subclass : public item_class
{
public:
  leather_boots_item_subclass() : item_class("Leather boots", "When used increases players speed by 1 by cushioning their steps.", 'b') {}
  void use(shared_ptr<player_class>& player_pointer_object)
  {
    cout << "You wore leather boots. Speed permanently increased by 1" << endl;
    player_pointer_object->player_speed++;
  }
};
fstream savefile_object;
// FUNCTION PROTOTYPE/DECLARATION
void menu();
void l2startgame();
void check_stats();
void check_items();
void check_skills();
void save();
void shoot_fireball();
vector<shared_ptr<obstacle_class>> obstacles_vector;
vector<shared_ptr<enemy_class>> enemies_vector;
vector<shared_ptr<item_class>> items_vector;
shared_ptr<player_class> player_pointer_object = make_shared<player_class>();
string version = "0.2.2";
bool music_variable = true; 
bool gameover = false;
int moneyx, moneyy; // money draw() position
int score = 0;
int lives = 3;
int difficulty=3;
int won_game = false;
int language=1; // language 1 = english
int level=1;
int level_select_variable=1; // for bonus level select
bool shoot_skill_cooldown = false;  // Flag to track the cooldown state
chrono::steady_clock::time_point lastShootTime;  // Track the last shoot time
void clear_screen()
{
#ifdef _WIN32
    system("cls");
#else
    cout << "\033[2J\033[1;1H";
#endif
}
int cin_valid_input()
{
  int input_variable;
  while(true)
  {
    cin >> input_variable;
    if(cin.fail())
    {
      cin.clear();
      cin.ignore();
      cout << "Invalid input. Try again: ";
    }
    else
    {
      break;
    }
  }
    cin.ignore();
    return input_variable;
}
void random_generate_obstacle()
{
  int max_obstacle_objects = 0;
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

  // Generate a random obstacle by randomly selecting obstacle subclass
  for (int i = 0; i < max_obstacle_objects; i++)
  {
    int obstacleType = rand() % 2;
    if (obstacleType == 0) // rock
    {
      shared_ptr<rock_obstacle_subclass> rock_obstacle = make_shared<rock_obstacle_subclass>();
      obstacles_vector.push_back(rock_obstacle);
    }
    else // tree
    {
      shared_ptr<tree_obstacle_subclass> tree_obstacle = make_shared<tree_obstacle_subclass>();
      obstacles_vector.push_back(tree_obstacle);
    }
  }
}
void random_generate_enemy()
{
  int max_enemy_objects = 0;
  if(map_size==1) // small
  {
    max_enemy_objects += 1;
    max_enemy_objects += rand() % 1;
  }
  else if(map_size==2) // medium
  {
    max_enemy_objects += 2;
    max_enemy_objects += rand() % 2;
  }
  else if(map_size==3) // large
  {
    max_enemy_objects += 3;
    max_enemy_objects += rand() % 3;
  }
  else if(map_size==4) // extra-large
  {
    max_enemy_objects += 4;
    max_enemy_objects += rand() % 4;
  }
  else if(map_size==5) // giant
  {
    max_enemy_objects += 5;
    max_enemy_objects += rand() % 5;
  }
  for (int i = 0; i < max_enemy_objects; i++)
  {
    int enemy_type_variable = rand() % 2;
    if (enemy_type_variable == 0) // xxx
    {
      shared_ptr<stalker_enemy_subclass> stalker_enemy = make_shared<stalker_enemy_subclass>();
      enemies_vector.push_back(stalker_enemy);
    }
    else // xxx
    {
      shared_ptr<stalker_enemy_subclass> stalker_enemy = make_shared<stalker_enemy_subclass>();
      enemies_vector.push_back(stalker_enemy);
    }
  }
}
void random_generate_items()
{
  int max_item_objects = 0;
  if(map_size==1) // small
  {
    max_item_objects += 5;
    max_item_objects += rand() % 5;
  }
  else if(map_size==2) // medium
  {
    max_item_objects += 10;
    max_item_objects += rand() % 5;
  }
  else if(map_size==3) // large
  {
    max_item_objects += 15;
    max_item_objects += rand() % 5;
  }
  else if(map_size==4) // extra-large
  {
    max_item_objects += 20;
    max_item_objects += rand() % 5;
  }
  else if(map_size==5) // giant
  {
    max_item_objects += 25;
    max_item_objects += rand() % 5;
  }
  for (int i = 0; i < max_item_objects; i++)
  {
    int item_type_variable = rand() % 2;
    if (item_type_variable == 0) // potion
    {
      shared_ptr<potion_item_subclass> leather_boots_item = make_shared<potion_item_subclass>();
      items_vector.push_back(make_shared<potion_item_subclass>());
    }
    else // leather boots
    {
      shared_ptr<leather_boots_item_subclass> leather_boots_item = make_shared<leather_boots_item_subclass>();
      items_vector.push_back(make_shared<leather_boots_item_subclass>());
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

  // to refresh vectors for new game and level select
  items_vector.clear();
  enemies_vector.clear();


  direction = STOP;
  player_pointer_object->player_x_pos = width / 2;
  player_pointer_object->player_y_pos = height / 2;

  random_generate_obstacle();

  enemies_vector.push_back(make_shared<fire_enemy_subclass>());
  enemies_vector.push_back(make_shared<flying_enemy_subclass>());
  
  items_vector.push_back(make_shared<potion_item_subclass>());
  items_vector.push_back(make_shared<leather_boots_item_subclass>());

  moneyx = rand() % width;
  moneyy = rand() % height;
  

  //initialise buffer with default character ' ' (space) to avoid console buffer not clearing.
  for (int i = 0; i < height; i++)
  {  
    for (int j = 0; j < width; j++)
    {
        buffer[i][j] = ' ';
    }
  }
}
void l2setup() 
{
  if (music_variable == false)
  {
    PlaySoundW(NULL, NULL, 0);
  }
  else
  {
    PlaySoundW(L"sound//music//alien-jungle.wav", NULL, SND_FILENAME | SND_ASYNC);
  }
  map_size=2;
  // reset the level variables
  score=0;
  gameover = false;

  // Below srand method needs to be executed at runtime hence run in a function vs global variable;
  moneyx = rand() % l2width;
  moneyy = rand() % l2height;

  // setting position of player
  direction = STOP;
  player_pointer_object->player_x_pos = l2width / 2;
  player_pointer_object->player_y_pos = l2height / 2;

  // to refresh vectors for new game and level select
  items_vector.clear();
  enemies_vector.clear();

  //initialise buffer with default character ' ' (space) to avoid console buffer not clearing.
  for (int i = 0; i < l2height; i++)
  {  
    for (int j = 0; j < l2width; j++)
    {
        l2buffer[i][j] = ' ';
    }
  }
}
void draw_level_1()
{
  // Initialize the buffer with wall boundaries and empty spaces
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      // Draw wall boundaries
      if (x == 0 || x == width - 1 || y == 0 || y == height - 1)
        buffer[y][x] = '#'; // draw all 4 walls
      else
        buffer[y][x] = ' '; // fill empty spaces with blank
    }
  }

  // Draw player
  buffer[player_pointer_object->player_y_pos][player_pointer_object->player_x_pos] = 'P';

  // Draw money
  buffer[moneyy][moneyx] = '$';

  // Draw obstacles
  for (const auto& obstacle : obstacles_vector)
  {
    if (obstacle->obstacle_alive)
    {
      buffer[obstacle->obstacle_y_pos][obstacle->obstacle_x_pos] = obstacle->obstacle_symbol;
    }
  }

  // Draw enemies
  for (const auto& enemy : enemies_vector) 
  {
    if (enemy->enemy_alive) 
    {
      buffer[enemy->enemy_y_pos][enemy->enemy_x_pos] = enemy->enemy_symbol;
    }
  }

  // Draw items
  for (const auto& item : items_vector) 
  {
    if (item->item_alive) 
    {
      buffer[item->item_y_pos][item->item_x_pos] = item->item_symbol;
    }
  }

  // Clear the console screen
  system("cls");

  // Print the buffer
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      cout << buffer[y][x];
    }
    cout << endl;
  }

  // Print the current score and lives beneath the array
  string score_str = "Current Score: " + to_string(score);
  string lives_str = "Current Lives: " + to_string(lives);
  cout << score_str << endl;
  cout << lives_str << endl;
}
void draw_level_2()
{
    // Draw top wall  
  for (int top_wall = 0; top_wall < l2width; top_wall++) {
    l2buffer[0][top_wall] = '#';
  }

  // draw middle section
  //loop through y axis 19 times down
  for (int y = 1; y < l2height-1; y++) 
  {
    // loop through x axis 19 times across
    for (int x = 0; x < l2width; x++) 
    {
      // draw side wall
      if (x == 0 || x == l2width - 1) 
      {
        l2buffer[y][x] = '#';
      }
      //draw player
      else if (x == player_pointer_object->player_x_pos && y == player_pointer_object->player_y_pos) 
      {
        l2buffer[y][x] = 'P';
      }
      //draw money
      else if (x == moneyx && y == moneyy) 
      {
        l2buffer[y][x] = '$';
      }
      else 
      {
        // Initialize to empty space
        l2buffer[y][x] = ' ';

        // Draw all enemies if any exists at this position
        for (const auto& enemy : enemies_vector) 
        {
          if (enemy->enemy_alive && x == enemy->enemy_x_pos && y == enemy->enemy_y_pos) 
          {
            l2buffer[y][x] = enemy->enemy_symbol;
          }
        }
      }
    } 
  }

  // Draw bottom wall
  for (int bottom_wall = 0; bottom_wall < l2width; bottom_wall++) {
    l2buffer[height-1][bottom_wall] = '#';
  }

  system("cls");
  for (int y = 0; y < l2height; y++) 
  {
    for (int x = 0; x < l2width; x++) {
      cout << l2buffer[y][x];
    }
    cout << endl;
  }
  // print the current score and lives beneath the array
  string score_str = "Current Score: " + to_string(score);
  string lives_str = "Current Lives: " + to_string(lives);
  cout << score_str << endl;
  cout << lives_str << endl;
}
void draw_level_3()
{
  cout << "Level 3 coming soon" << endl;
  menu();
}
void draw_level_4()
{
    cout << "Test" << endl;
}
void draw_level_5()
{
    cout << "Test" << endl;
}
void draw_level_6()
{
    cout << "Test" << endl;
}
void draw_level_7()
{
    cout << "Test" << endl;
}
void draw_level_8()
{
    cout << "Test" << endl;
}
void draw_level_9()
{
    cout << "Test" << endl;
}
void draw_level_10()
{
    cout << "Test" << endl;
}
void draw_level_11()
{
    cout << "Test" << endl;
}
void input()
{
    if (_kbhit()) 
    {
        switch (_getch()) 
        {
        case 'w':
            direction = UP;
            break;
        case 's':
            direction = DOWN;
            break;
        case 'a':
            direction = LEFT;
            break;
        case 'd':
            direction = RIGHT;
            break;
        case 'i':
            check_items();
            cout << "Press ENTER button to Return to game" << endl;
            cin.get();
            break;
        case 'l':
            check_objective();
            cout << "Press ENTER button to Return to game" << endl;
            cin.get();
            break;
        case 'x':
            check_skills();
            cout << "Press ENTER button to Return to game" << endl;
            cin.get();
            break; 
        case 'c':
            check_stats();
            cout << "Press ENTER button to Return to game" << endl;
            cin.get();
            break; 
        case 59:
            save();
            break;
        case 27: // escape key
            menu();
            break;
        case 'q':
            exit(0);
            break;
        case ' ':
            if (direction != STOP)
            {
                chrono::steady_clock::time_point currentTime = chrono::steady_clock::now();
                chrono::duration<double> elapsedSeconds = currentTime - lastShootTime;
                if (elapsedSeconds.count() >= 2.0) {
                    // The shoot skill is off cooldown
                    shoot_fireball();
                    // Update the last shoot time
                } else {
                    // The shoot skill is on cooldown
                    cout << "The shoot skill is on cooldown. Please wait for " << (2.0 - elapsedSeconds.count()) << " seconds." << endl;
                }
            }
            break;
        case 'e':
            direction = STOP;
            break;
        }
    }
}
void POSIXinput()
{
    if (_kbhit()) 
    {
        switch (_getch()) 
        {
        case 'w':
            direction = UP;
            break;
        case 's':
            direction = DOWN;
            break;
        case 'a':
            direction = LEFT;
            break;
        case 'd':
            direction = RIGHT;
            break;
        case 'i':
            check_items();
            cout << "Press ENTER button to Return to game" << endl;
            cin.get();
            break;
        case 'l':
            check_objective();
            cout << "Press ENTER button to Return to game" << endl;
            cin.get();
            break;
        case 'x':
            check_skills();
            cout << "Press ENTER button to Return to game" << endl;
            cin.get();
            break; 
        case 'c':
            check_stats();
            cout << "Press ENTER button to Return to game" << endl;
            cin.get();
            break; 
        case 59:
            save();
            break;
        case 27: // escape key
            menu();
            break;
        case 'q':
            exit(0);
            break;
        case ' ':
            if (direction != STOP)
            {
                chrono::steady_clock::time_point currentTime = chrono::steady_clock::now();
                chrono::duration<double> elapsedSeconds = currentTime - lastShootTime;
                if (elapsedSeconds.count() >= 2.0) {
                    // The shoot skill is off cooldown
                    shoot_fireball();
                    // Update the last shoot time
                } else {
                    // The shoot skill is on cooldown
                    cout << "The shoot skill is on cooldown. Please wait for " << (2.0 - elapsedSeconds.count()) << " seconds." << endl;
                }
            }
            break;
        case 'e':
            direction = STOP;
            break;
        }
    }
}
void save()
{
    std::cout << "Saving Game..." << std::endl;
    cin.get();
  
    // search strings to match
    string name_match = "Name: ";
    string magic_match = "Magic: ";
    string health_match = "Health: ";
    string xp_match = "XP: ";
    string speed_match = "Speed: ";
    string literacy_match = "Literacy: ";
    string diplomacy_match = "Diplomacy: ";
    string swimming_match = "Swimming: ";
    string herbology_match = "Herbology: ";
    string money_match = "Money: ";

    // test open file to read the contents first
    savefile_object.open("magian_save.txt", ios::app);
    if (savefile_object.is_open())
    {
      string line;
      while (getline(savefile_object, line))
      {
        if (line.find(name_match) != string::npos) // NAME
        {
          savefile_object << name_match << player_pointer_object->player_name;
        }
        if (line.find(magic_match) != string::npos) // MAGIC
        {
          savefile_object << magic_match << player_pointer_object->player_magic;
        }
        if (line.find(health_match) != string::npos) // HEALTH
        {
          savefile_object << health_match << player_pointer_object->player_health;
        }
        if (line.find(xp_match) != string::npos) // XP
        {
          savefile_object << xp_match << player_pointer_object->player_xp;
        }
        if (line.find(speed_match) != string::npos) // SPEED
        {
          savefile_object << speed_match << player_pointer_object->player_speed;
        }
        if (line.find(literacy_match) != string::npos) // LITERACY
        {
          savefile_object << literacy_match << player_pointer_object->player_literacy;
        }
        if (line.find(diplomacy_match) != string::npos) // DIPLOMACY
        {
          savefile_object << diplomacy_match << player_pointer_object->player_diplomacy;
        }
        if (line.find(swimming_match) != string::npos) // SWIMMING
        {
          savefile_object << swimming_match << player_pointer_object->player_swimming;
        }
        if (line.find(herbology_match) != string::npos) // HERBOLOGY
        {
          savefile_object << herbology_match << player_pointer_object->player_herbology;
        }  
        if (line.find(money_match) != string::npos) // Money
        {
          savefile_object << money_match << player_pointer_object->player_money;
        }    
      }
      savefile_object.close();
      std::cout << "Game Saved" << std::endl;
      cin.get();
    }
    // test open file to read the contents first
    else
    {
        cerr << "Error: failed to open magian_save.txt" << endl;
        return;
    }
}
void collision_logic()
{
  player_pointer_object->previous_x_pos = player_pointer_object->player_x_pos;
  player_pointer_object->previous_y_pos = player_pointer_object->player_y_pos;

  // Enemies
  for (const auto& enemy : enemies_vector)
  {
    if (enemy->enemy_alive && player_pointer_object->player_x_pos == enemy->enemy_x_pos && player_pointer_object->player_y_pos == enemy->enemy_y_pos)
    {
      player_pointer_object->player_x_pos = player_pointer_object->previous_x_pos;
      player_pointer_object->player_y_pos = player_pointer_object->previous_y_pos;

      enemy->enemy_check_collision(player_pointer_object);
    } else {  }
  }

  // Obstacles
  for (const auto& obstacle : obstacles_vector)
  {
    if (obstacle->obstacle_alive && player_pointer_object->player_x_pos == obstacle->obstacle_x_pos && player_pointer_object->player_y_pos == obstacle->obstacle_y_pos)
    {
      player_pointer_object->player_x_pos = player_pointer_object->previous_x_pos;
      player_pointer_object->player_y_pos = player_pointer_object->previous_y_pos;
      obstacle->obstacle_check_collision(player_pointer_object);
    } else {  }
  }

  // Items
  for(const auto item : items_vector)
  {
    if (player_pointer_object->player_x_pos == item->item_x_pos && player_pointer_object->player_y_pos == item->item_y_pos) 
    {
      player_pointer_object->player_x_pos = player_pointer_object->previous_x_pos;
      player_pointer_object->player_y_pos = player_pointer_object->previous_y_pos;
      item->item_check_collision(player_pointer_object);
    } else {  }
  }
}
void win_logic()
{
// Check if player has run out of lives and end the game if true
    if (lives <= 0) 
    {
        cout << "You died!" << endl;
        gameover = true;
        menu();
    } else {  }

// Level 1 - win logic 
    if (score >= 3) 
    {
    level = 2;
    cout << "You win the level";
    level_select_variable=2;
    update_savefile_level();
    save();
    l2startgame();
    } else {  }

    // Level 2 - win logic - after specific time searching win condition (friend? or lover) appears
    if (score >= 10) 
    {
    level = 3;
    cout << "You win the level";
    level_select_variable=3;
    update_savefile_level();
    save();
    } else {  }

   // Level 3 - win logic - e.g. specific enemy dies
    if (score >= 20) 
    {
      level = 4;
      cout << "You win the level";
      level_select_variable=4;
      update_savefile_level();
      save();
    } else {  }

    // Win game logic
    if (level = 12) 
    {
      won_game = true;
      cout << "You have become the greatest of magicians. The ultimate scholar on the worlds"
              "greatest languages and spells. Now the greatest challenge lies ahead to make your own game"
              "and spread the joy of language";
    } else {  }

}
void levelup_logic()
{
if (player_pointer_object->player_xp > 3) // level 1
  {
    cout << "Your experience and knowledge gained throughout life increases your capability";
    player_pointer_object->player_magic += 1;
    player_pointer_object->player_health += 3;
  }
  else if (player_pointer_object->player_xp > 6) // level 2
  {
    cout << "Your experience and knowledge gained throughout life increases your capability";
    player_pointer_object->player_magic += 1;
    player_pointer_object->player_health += 3;
  }
  else if (player_pointer_object->player_xp > 9) // level 3
  {
    cout << "Your experience and knowledge gained throughout life increases your capability";
    player_pointer_object->player_magic += 1;
    player_pointer_object->player_health += 3;
  }
}
void enemy_ai_logic()
{
    // Fire enemy will move slow
    enemies_vector[0]->random_slow_movement();
    // Flying Rakshaa enemy will move fast
    enemies_vector[1]->random_fast_movement();
    // Stalking Rakshaa enemy will hunt player slowly
    // enemies_vector[0]->random_slow_chasing(player_pointer_object);
}
void money_pickup_logic()
{
// Level 1 score - Check if player picked up money and update score and money location if true
    if (player_pointer_object->player_x_pos == moneyx && player_pointer_object->player_y_pos == moneyy) 
    {
        score++;
        player_pointer_object->player_money++;
        moneyx = rand() % width-1;
        moneyy = rand() % height-1;
    } else {  }

    // Level 2 score - Check if player picked up money and update score and money location if true
    if (player_pointer_object->player_x_pos == moneyx && player_pointer_object->player_y_pos == moneyy) 
    {
        score++;
        player_pointer_object->player_money++;
        moneyx = rand() % l2width-1;
        moneyy = rand() % l2height-1;
    } else {  }
}
void logic() 
{
  // New direction in buffer updated called after input() in startgame() loop.
  // if condition prevents moving out of bounds #
  switch (direction) 
  {
    case UP:
        if (player_pointer_object->player_y_pos > 1 && buffer[player_pointer_object->player_y_pos - 1][player_pointer_object->player_x_pos] != '#') {
        player_pointer_object->player_y_pos--; }
        break;
    case DOWN:
    if (player_pointer_object->player_y_pos < height - 2 && buffer[player_pointer_object->player_y_pos + 1][player_pointer_object->player_x_pos] != '#') {
        player_pointer_object->player_y_pos++; }
        break;
    case LEFT:
    if (player_pointer_object->player_x_pos > 1 && buffer[player_pointer_object->player_y_pos][player_pointer_object->player_x_pos - 1] != '#') {
        player_pointer_object->player_x_pos--; }
        break;
    case RIGHT:
    if (player_pointer_object->player_x_pos < width - 2 && buffer[player_pointer_object->player_y_pos][player_pointer_object->player_x_pos + 1] != '#') {
        player_pointer_object->player_x_pos++; }
        break;
  }

    collision_logic();
    enemy_ai_logic();
    win_logic();
    levelup_logic();
    money_pickup_logic();

}
void l2startgame() 
{
  l2setup();
  lives = 3;
  
  if (host_OS_name_variable == "Windows")
  { 
    while (!gameover) 
    {
      draw_level_2();
      input();
      logic();
      Sleep(150);
    }
    cout << "Game Over. Your final score is: " << score << endl;
    cin.get();
  }
  else
  {
    while (!gameover) 
    {
      draw_level_2();
      POSIXinput();
      logic();
      Sleep(150);
    }
    cout << "Game Over. Your final score is: " << score << endl;
    cin.get();
  }
}
void shoot_fireball()
{

    if (shoot_skill_cooldown) 
    {
        cout << "Your too tired to shoot again.";
        return;
    }

    // The bullet STARTS from the players current position
    int targetX = player_pointer_object->player_x_pos;
    int targetY = player_pointer_object->player_y_pos;

    while (true) 
    {
        // Move the target position based on the player's direction
        if (direction == UP)
            targetY--;
        else if (direction == DOWN)
            targetY++;
        else if (direction == LEFT)
            targetX--;
        else if (direction == RIGHT)
            targetX++;

        // Check if the target position is within the bounds of the game map
        if (targetX >= 0 && targetX < width && targetY >= 0 && targetY < height) 
        {
            // Check if there is an enemy at the target position
            bool hitEnemy = false;
            for (const auto& enemy : enemies_vector) 
            {
                if (enemy->enemy_alive && targetX == enemy->enemy_x_pos && targetY == enemy->enemy_y_pos) 
                {
                    hitEnemy = true;
                    enemy->enemy_hp--;
                    // If enemy's health is 0 or less, set the enemy's alive property to false
                    if (enemy->enemy_hp <= 0) 
                    {
                        player_pointer_object->player_xp += enemy->enemy_xp;
                        enemy->enemy_alive = false;
                    }
                    break;
                }
            }

            // Check if there is an obstacle at the target position
            bool hit_obstacle = false;
            for (const auto& obstacle : obstacles_vector) 
            {
                if (obstacle->obstacle_alive && targetX == obstacle->obstacle_x_pos && targetY == obstacle->obstacle_y_pos) 
                {
                    hit_obstacle = true;
                    obstacle->obstacle_hp--;
                    // If enemy's health is 0 or less, set the enemy's alive property to false
                    if (obstacle->obstacle_hp <= 0) 
                    {
                        obstacle->obstacle_alive = false;
                    }
                    break;
                }
            }

            // Clear the bullet from the previous position
            buffer[player_pointer_object->player_y_pos][player_pointer_object->player_x_pos] = ' ';

            // Draw the bullet at the target position
            buffer[targetY][targetX] = '*';
            draw_level_1();

            // Sleep after drawing the bullet
            Sleep(50);

            // Clear the bullet from the current position
            buffer[targetY][targetX] = ' ';

            // Check if the target position hit a wall
            if (buffer[targetY][targetX] == '#') 
            {
                break;
            }
        } 
        else 
        {
            break;
        }
    }
    // shoot skill cooldown
    shoot_skill_cooldown = true;
    lastShootTime = chrono::steady_clock::now();
    // reset shoot skill cooldown
    shoot_skill_cooldown = false;
}
void volcano()
{
  // skill to shoot in all 4 directions

    if (shoot_skill_cooldown) 
    {
        cout << "Your too tired to shoot again.";
        return;
    }

    // The bullet STARTS from the players current position
    int targetX = player_pointer_object->player_x_pos;
    int targetY = player_pointer_object->player_y_pos;

    while (true) 
    {
        // Move the target position based on the player's direction
            targetY--;
        // Check if the target position is within the bounds of the game map
        if (targetX >= 0 && targetX < width && targetY >= 0 && targetY < height) 
        {
            // Check if there is an enemy at the target position
            bool hitEnemy = false;
            for (const auto& enemy : enemies_vector) 
            {
                if (enemy->enemy_alive && targetX == enemy->enemy_x_pos && targetY == enemy->enemy_y_pos) 
                {
                    hitEnemy = true;
                    enemy->enemy_hp--;
                    // If enemy's health is 0 or less, set the enemy's alive property to false
                    if (enemy->enemy_hp <= 0) 
                    {
                        player_pointer_object->player_xp += enemy->enemy_xp;
                        enemy->enemy_alive = false;
                    }
                    break;
                }
            }

            // Check if there is an obstacle at the target position
            bool hit_obstacle = false;
            for (const auto& obstacle : obstacles_vector) 
            {
                if (obstacle->obstacle_alive && targetX == obstacle->obstacle_x_pos && targetY == obstacle->obstacle_y_pos) 
                {
                    hit_obstacle = true;
                    obstacle->obstacle_hp--;
                    // If enemy's health is 0 or less, set the enemy's alive property to false
                    if (obstacle->obstacle_hp <= 0) 
                    {
                        obstacle->obstacle_alive = false;
                    }
                    break;
                }
            }

            // Clear the bullet from the previous position
            buffer[player_pointer_object->player_y_pos][player_pointer_object->player_x_pos] = ' ';

            // Draw the bullet at the target position
            buffer[targetY][targetX] = '*';
            draw_level_1();

            // Sleep after drawing the bullet
            Sleep(50);

            // Clear the bullet from the current position
            buffer[targetY][targetX] = ' ';

            // Check if the target position hit a wall
            if (buffer[targetY][targetX] == '#') 
            {
                break;
            }
        } 
        else 
        {
            break;
        }
    }

    // left shoot side

  while (true) 
    {
        // Move the target position based on the player's direction
            targetX--;

        // Check if the target position is within the bounds of the game map
        if (targetX >= 0 && targetX < width && targetY >= 0 && targetY < height) 
        {
            // Check if there is an enemy at the target position
            bool hitEnemy = false;
            for (const auto& enemy : enemies_vector) 
            {
                if (enemy->enemy_alive && targetX == enemy->enemy_x_pos && targetY == enemy->enemy_y_pos) 
                {
                    hitEnemy = true;
                    enemy->enemy_hp--;
                    // If enemy's health is 0 or less, set the enemy's alive property to false
                    if (enemy->enemy_hp <= 0) 
                    {
                        player_pointer_object->player_xp += enemy->enemy_xp;
                        enemy->enemy_alive = false;
                    }
                    break;
                }
            }

            // Check if there is an obstacle at the target position
            bool hit_obstacle = false;
            for (const auto& obstacle : obstacles_vector) 
            {
                if (obstacle->obstacle_alive && targetX == obstacle->obstacle_x_pos && targetY == obstacle->obstacle_y_pos) 
                {
                    hit_obstacle = true;
                    obstacle->obstacle_hp--;
                    // If enemy's health is 0 or less, set the enemy's alive property to false
                    if (obstacle->obstacle_hp <= 0) 
                    {
                        obstacle->obstacle_alive = false;
                    }
                    break;
                }
            }

            // Clear the bullet from the previous position
            buffer[player_pointer_object->player_y_pos][player_pointer_object->player_x_pos] = ' ';

            // Draw the bullet at the target position
            buffer[targetY][targetX] = '*';
            draw_level_1();

            // Sleep after drawing the bullet
            Sleep(50);

            // Clear the bullet from the current position
            buffer[targetY][targetX] = ' ';

            // Check if the target position hit a wall
            if (buffer[targetY][targetX] == '#') 
            {
                break;
            }
        } 
        else 
        {
            break;
        }
    }

    // right shoot side

    while (true) 
    {
        // Move the target position based on the player's direction
            targetX++;

        // Check if the target position is within the bounds of the game map
        if (targetX >= 0 && targetX < width && targetY >= 0 && targetY < height) 
        {
            // Check if there is an enemy at the target position
            bool hitEnemy = false;
            for (const auto& enemy : enemies_vector) 
            {
                if (enemy->enemy_alive && targetX == enemy->enemy_x_pos && targetY == enemy->enemy_y_pos) 
                {
                    hitEnemy = true;
                    enemy->enemy_hp--;
                    // If enemy's health is 0 or less, set the enemy's alive property to false
                    if (enemy->enemy_hp <= 0) 
                    {
                        player_pointer_object->player_xp += enemy->enemy_xp;
                        enemy->enemy_alive = false;
                    }
                    break;
                }
            }

            // Check if there is an obstacle at the target position
            bool hit_obstacle = false;
            for (const auto& obstacle : obstacles_vector) 
            {
                if (obstacle->obstacle_alive && targetX == obstacle->obstacle_x_pos && targetY == obstacle->obstacle_y_pos) 
                {
                    hit_obstacle = true;
                    obstacle->obstacle_hp--;
                    // If enemy's health is 0 or less, set the enemy's alive property to false
                    if (obstacle->obstacle_hp <= 0) 
                    {
                        obstacle->obstacle_alive = false;
                    }
                    break;
                }
            }

            // Clear the bullet from the previous position
            buffer[player_pointer_object->player_y_pos][player_pointer_object->player_x_pos] = ' ';

            // Draw the bullet at the target position
            buffer[targetY][targetX] = '*';
            draw_level_1();

            // Sleep after drawing the bullet
            Sleep(50);

            // Clear the bullet from the current position
            buffer[targetY][targetX] = ' ';

            // Check if the target position hit a wall
            if (buffer[targetY][targetX] == '#') 
            {
                break;
            }
        } 
        else 
        {
            break;
        }
    }

    // down shoot side

    while (true) 
    {
        // Move the target position based on the player's direction
            targetY++;

        // Check if the target position is within the bounds of the game map
        if (targetX >= 0 && targetX < width && targetY >= 0 && targetY < height) 
        {
            // Check if there is an enemy at the target position
            bool hitEnemy = false;
            for (const auto& enemy : enemies_vector) 
            {
                if (enemy->enemy_alive && targetX == enemy->enemy_x_pos && targetY == enemy->enemy_y_pos) 
                {
                    hitEnemy = true;
                    enemy->enemy_hp--;
                    // If enemy's health is 0 or less, set the enemy's alive property to false
                    if (enemy->enemy_hp <= 0) 
                    {
                        player_pointer_object->player_xp += enemy->enemy_xp;
                        enemy->enemy_alive = false;
                    }
                    break;
                }
            }

            // Check if there is an obstacle at the target position
            bool hit_obstacle = false;
            for (const auto& obstacle : obstacles_vector) 
            {
                if (obstacle->obstacle_alive && targetX == obstacle->obstacle_x_pos && targetY == obstacle->obstacle_y_pos) 
                {
                    hit_obstacle = true;
                    obstacle->obstacle_hp--;
                    // If enemy's health is 0 or less, set the enemy's alive property to false
                    if (obstacle->obstacle_hp <= 0) 
                    {
                        obstacle->obstacle_alive = false;
                    }
                    break;
                }
            }

            // Clear the bullet from the previous position
            buffer[player_pointer_object->player_y_pos][player_pointer_object->player_x_pos] = ' ';

            // Draw the bullet at the target position
            buffer[targetY][targetX] = '*';
            draw_level_1();

            // Sleep after drawing the bullet
            Sleep(50);

            // Clear the bullet from the current position
            buffer[targetY][targetX] = ' ';

            // Check if the target position hit a wall
            if (buffer[targetY][targetX] == '#') 
            {
                break;
            }
        } 
        else 
        {
            break;
        }
    }


    // shoot skill cooldown
    shoot_skill_cooldown = true;
    lastShootTime = chrono::steady_clock::now();
    // reset shoot skill cooldown
    shoot_skill_cooldown = false;
}
void check_items()
{
  // Read from the savefile
  cout << "INVENTORY" << endl;
  for (int i = 0; i < items_vector.size(); i++)
  {
    cout << i << ": " << items_vector[i]->item_name << endl;
  }

  // Using an item
  int item_select_variable;
  cout << "Select item you wish to use: " << endl;
  cin >> item_select_variable;

  if (item_select_variable >= 0 && item_select_variable < items_vector.size())
  {
    cout << "Using item: " << items_vector[item_select_variable]->item_name << endl;
    
    // Find the item by name and call its use() function
    for (const auto& item : items_vector)
    {
      if (item->item_name == items_vector[item_select_variable]->item_name)
      {
        item->use(lives);
        break;
      }
    }
    
    // Remove the item from the items_vector
    items_vector.erase(items_vector.begin() + item_select_variable);

    cout << "Press ENTER to continue...";
    cin.get();
  }
  else
  {
    cout << "Invalid item index option" << endl;
  }
}
void item_store_header()
{
  savefile_object.open("magian_save.txt", ios::app);
  if(savefile_object.is_open())
  {
    savefile_object << "INVENTORY" << endl;
    savefile_object.close();
  }
  else
  {
    cerr << "Error: Couldn't write header txt INVENTORY to savefile";
  }
}
void check_skills()
{
  // read from the savefile NOT the pointer object!!!
  cout << "SKILLS\n\n"
  "Change skill to\n";
}
void check_stats()
{
  cout << "STATS\n";
  string a;
  int b;
  int c;
  int d;
  int e;
  int f;
  int g;
  int h;
  int i;
  int j;

  savefile_object.open("magian_save.txt", ios::in);
    if (savefile_object.is_open())
    {
      string line;
      while (getline(savefile_object, line))
      {
        if (line.find("Name: ") != string::npos)
          a = stoi(line.substr(6));

        else if (line.find("Magic: ") != string::npos)
          b = stoi(line.substr(7));

        else if (line.find("Health: ") != string::npos)
          c = stoi(line.substr(8));

        else if (line.find("XP: ") != string::npos)
          f = stoi(line.substr(4));

        else if (line.find("Speed: ") != string::npos)
          e = stoi(line.substr(7));

        else if (line.find("Literacy: ") != string::npos)
          f = stoi(line.substr(10));

        else if (line.find("Diplomacy: ") != string::npos)
          g = stoi(line.substr(11));

        else if (line.find("Swimming: ") != string::npos)
          h = stoi(line.substr(10));
          
        else if (line.find("Herbology: ") != string::npos)
          i = stoi(line.substr(11));

        else if (line.find("Money: ") != string::npos)
          j = stoi(line.substr(7));
      }
        savefile_object.close();

        // Print the stats
        cout << "Player Stats:" << endl;
        cout << "Name: " << a << endl;
        cout << "Magic: " << b << endl;
        cout << "Health: " << c << endl;
        cout << "XP: " << f << endl;
        cout << "Speed: " << e << endl;
        cout << "Literacy: " << f << endl;
        cout << "Diplomacy: " << g << endl;
        cout << "Swimming: " << h << endl;
        cout << "Herbology: " << i << endl;
        cout << "Money: " << i << endl;
        
    }
    else
    {
        cerr << "Error: Unable to open save file." << endl;
    }
}
void setup_player_header()
{
  savefile_object.open("magian_save.txt", ios::app);
  if(savefile_object.is_open())
  {
    savefile_object << "AVATAR" << endl;
    savefile_object.close();
  }
  else
  {
    cerr << "Error: Couldn't write header txt AVATAR to savefile";
  }
}
void initialise_player()
{
  int player_magic = 1;
  int player_health = 3;
  int player_literacy = 1;
  int player_diplomacy = 0;
  int player_swimming = 0;
  int player_herbology = 0;
  int player_xp = 0;
  int player_speed = 1;
  int player_money = 0;
  player_pointer_object->player_magic = player_magic;
  player_pointer_object->player_health = player_health;
  player_pointer_object->player_xp = player_xp;
  player_pointer_object->player_speed = player_speed;
  player_pointer_object->player_literacy = player_literacy;
  player_pointer_object->player_diplomacy = player_diplomacy;
  player_pointer_object->player_swimming = player_swimming;
  player_pointer_object->player_herbology = player_herbology;
  player_pointer_object->player_money = player_money;
  
  savefile_object.open("magian_save.txt", ios::app);
    if(savefile_object.is_open())
    {
        savefile_object << "Magic: " << player_magic << endl;
        savefile_object << "Health: " << player_health << endl;
        savefile_object << "XP: " << player_speed << endl;
        savefile_object << "Speed: " << player_speed << endl;
        savefile_object << "Literacy: " << player_literacy << endl;
        savefile_object << "Diplomacy: " << player_diplomacy << endl;
        savefile_object << "Swimming: " << player_swimming << endl;
        savefile_object << "Herbology: " << player_herbology << endl;
        savefile_object << "Money: " << player_money << endl;
        savefile_object.close();
    }
    else
    {
        cerr << "Error: Inventory not saved to save file" << endl;
        return;
    }
}
void load_player(shared_ptr<player_class>& player_pointer_object)
{
  savefile_object.open("magian_save.txt", ios::in);
    if (savefile_object.is_open())
    {
      string line;
      while (getline(savefile_object, line))
      {
        if (line.find("Name: ") != string::npos)
          player_pointer_object->player_name = stoi(line.substr(6));
        else if (line.find("Magic: ") != string::npos)
          player_pointer_object->player_magic = stoi(line.substr(7));
        else if (line.find("Health: ") != string::npos)
          player_pointer_object->player_health = stoi(line.substr(8));
        else if (line.find("XP: ") != string::npos)
          player_pointer_object->player_xp = stoi(line.substr(4));
        else if (line.find("Speed: ") != string::npos)
          player_pointer_object->player_speed = stoi(line.substr(7));
        else if (line.find("Literacy: ") != string::npos)
          player_pointer_object->player_literacy = stoi(line.substr(10));
        else if (line.find("Diplomacy: ") != string::npos)
          player_pointer_object->player_diplomacy = stoi(line.substr(11));
        else if (line.find("Swimming: ") != string::npos)
          player_pointer_object->player_swimming = stoi(line.substr(10));
        else if (line.find("Herbology: ") != string::npos)
          player_pointer_object->player_herbology = stoi(line.substr(11));
      }
        savefile_object.close();
    }
    else
    {
        cerr << "Error: Unable to open save file." << endl;
    }
}
void library()
{
  cout << "Choose a unlocked book to read\n\n"
  "1. Learn Sanskrit - Sumeet Singh\n"
  "2. Rig Veda - Vyasa\n"
  "3. Puranas - Vyasa"
  "4. Mahabharata - Vyasa" 
  "5. Gathas - Zarasthura\n"
  "6. Code of the Nesilim - Hittite\n"
  "7. Diary of Merer - Merer\n"
  "8. The Taking of Joppa - Egypt\n"
  "9. Book of the Dead - Egypt\n"
  "10. Code of Ur-Nammu - Summeria\n"
  "11. Laws of Eshnunna - Akkadian\n"
  "12. Code of Hammurabi - Akkadian\n"
  "13. Epic of Gilgamesh - Akkadian\n"
  "Select a book to read. ENTER 0 to exit: ";
  int book_variable = cin_valid_input();
  switch (book_variable)
  {
  case 0:
    menu();
  case 1:
    cout << "1. Learn Sanskrit - Sumeet Singh\n";
    library();
    break;
  case 2:
    cout << "2. Rig Veda - Vyasa\n";
    library();
    break;
  case 3:
    cout << "3. Puranas - Vyasa";
    library();
    break;
  case 4:
    cout << "4. Mahabharata - Vyasa";
    library();
    break;
  case 5:
    cout << "3. Gathas - Iran\n";
    library();
    break;
  case 6:
    cout << "4. Code of the Nesilim - Hittite\n";
    library();
    break;
  case 7:
    cout << "5. Diary of Merer - Egypt\n";
    library();
    break;
  case 8:
    cout << "6. The Taking of Joppa - Egypt\n";
    library();
    break;
  case 9:
    cout << "7. Book of the Dead - Egypt\n";
    library();
    break;
  case 10:
    cout << "8. Code of Ur-Nammu - Summeria\n";
    library();
    break;
  case 11:
    cout << "9. Laws of Eshnunna - Akkadian\n";
    library();
    break;
  case 12:
    cout << "10. Code of Hammurabi - Akkadian\n";
    library();
    break;
  case 13:
    cout << "11. Epic of Gilgamesh - Akkadian\n";
    library();
    break;
  default: 
    cout << "Incorrect input. Try again: ";
    library();
    break;
  }
  
}
void soundtrack()
{
  int soundtrack_option_variable;

  cout << "SOUNDTRACK\n\n"
  "Theme title song\n"
  "1. Cyber Attack - Julius H\n"
  "Level 1\n"
  "2. Alien Jungle remix\n"
  "Level 2\n"
  "3. Shima Uta Seige remix\n"
  "Level 3\n"
  "\n"
  "Level 4\n"
  "\n"
  "Level 5\n"
  "\n"
  "Level 6\n"
  "\n"
  "Level 7\n"
  "\n"
  "Level 8\n"
  "\n"
  "Level 9\n"
  "\n"
  "Level 10\n"
  "\n"
  "Level 11 Bonus\n"
  "\n\n"
  "Choose an option (ENTER 0 to exit): ";

  cin >> soundtrack_option_variable;
  switch(soundtrack_option_variable)
  {
    case 0:
      menu();
      break;
    case 1:
      PlaySoundW(NULL, NULL, 0);
      PlaySoundW(L"sound//music//Cyber_Attack_by_JuliusH.wav", NULL, SND_FILENAME | SND_ASYNC);
      cin.get();
      break;
    case 2:
      PlaySoundW(NULL, NULL, 0);
      PlaySoundW(L"sound//music//alien-jungle.wav", NULL, SND_FILENAME | SND_ASYNC);
      cin.get();
      break;
    case 3:
      PlaySoundW(NULL, NULL, 0);
      PlaySoundW(L"sound//music//shima-uta_seige.wav", NULL, SND_FILENAME | SND_ASYNC);
      cin.get();
      break;
    case 4:
      break;
    case 5:
      break;
    case 6:
      break;
    case 7:
      break;
    case 8:
      break;
    case 9:
      break;
    case 10:
      break;
    case 11:
      break;
    default:
      "Choose an option (ENTER 0 to exit): ";
      soundtrack();
      break;
  }
  soundtrack();
}
void welcome() 
{ 
  cout << "\n\n"
  "             ▄▄▄▄███▄▄▄▄      ▄████████    ▄██████▄   ▄█     ▄████████ ███▄▄▄▄ \n" 
  "           ▄██▀▀▀███▀▀▀██▄   ███    ███   ███    ███ ███    ███    ███ ███▀▀▀██▄\n" 
  "           ███   ███   ███   ███    ███   ███    █▀  ███▌   ███    ███ ███   ███\n" 
  "           ███   ███   ███   ███    ███  ▄███        ███▌   ███    ███ ███   ███\n" 
  "           ███   ███   ███ ▀███████████ ▀▀███ ████▄  ███▌ ▀███████████ ███   ███\n" 
  "           ███   ███   ███   ███    ███   ███    ███ ███    ███    ███ ███   ███\n" 
  "           ███   ███   ███   ███    ███   ███    ███ ███    ███    ███ ███   ███\n" 
  "             █   ███   █▀    ███    █▀    ████████▀  █▀     ███    █▀   ▀█   █▀ \n"                                                                        
  "\n\n\n" 
  "A game of mages from the school of Agni in Arianna. \n"
  "Collect from scrolls, herbs, gold and items to build your player\n"
  "and advance through the level.\n"
  "but dont fall off the path less you die! \n\n\n"
  "MAIN MENU\n"
  "1. Start Game\n"
  "2. Save | Load Game\n"
  "3. Level select\n"
  "4. Help\n"
  "5. Settings\n"
  "6. Library\n"
  "7. Soundtrack\n\n"
  "Choose an option (ENTER 0 to exit): ";
}
void help() 
{
  fstream readme_object;
  readme_object.open("readme.txt", ios::in);
  if (readme_object.is_open())
  {
    string line;
    while(getline(readme_object, line))
    {
      cout << line << endl;
    }
    readme_object.close();
  }
  else
  {
    cout << "No readme.txt file in project folder" << endl;
  }
  cout << "Press ENTER to continue...";
  cin.get();
  menu();
}
void choose_player_name()
{
  string player_name = "";
  bool name_correct_variable = false;
    while (name_correct_variable == false)
    {        
        cout << "Enter your characters name: ";
        getline(cin, player_name);
        if (player_name == "q" || player_name == "Q")
        {
            menu();
            return;
        }
        else if (player_name.length() <= 30)
        {
            player_pointer_object->player_name = player_name;
            
            name_correct_variable = true;
        }
        else 
        {
            cout << "Name must be less then 30 characters long\n"
            "Enter a new name again or enter q to quit: ";
        }
    }
    cout << "Your name is: " << player_name << endl;
    savefile_object.open("magian_save.txt", ios::app);
    if (savefile_object.is_open())
    {
        savefile_object << "NAME: " << player_name << endl;
        savefile_object.close();
    }
    else
    {
        cerr << "Error: failed to write character name to file" << endl;
        return;
    }
}
void newgame()
{
  setup_player_header();
  choose_player_name();
  initialise_player();
  item_store_header();
  setup();
  lives = 3;

  // Clear the console screen initially
  system("cls");

  // Print the static elements that don't change during gameplay
  draw_level_1();
  
  // Start the game loop
  if (host_OS_name_variable == "Windows")
  { 
    while (!gameover)
    {
      // Update the dynamic elements and player input
      input();
      logic();

      // Print the updated game state without clearing the screen
      draw_level_1();

      Sleep(150);
    }
    cout << "Game Over. Your final score is: " << score << endl;
    cin.get();
  }
  else
  {
    while (!gameover)
    {
      // Update the dynamic elements and player input
      POSIXinput();
      logic();

      // Print the updated game state without clearing the screen
      draw_level_1();

      Sleep(150);
    }
    cout << "Game Over. Your final score is: " << score << endl;
    cin.get();
  }
}
void continuegame() 
{
  load_player(player_pointer_object);
  setup();
  lives = 3;

  // Clear the console screen initially
  system("cls");

  // Print the static elements that don't change during gameplay
  draw_level_1();
  
  // Start the game loop
  if (host_OS_name_variable == "Windows")
  { 
    while (!gameover)
    {
      // Update the dynamic elements and player input
      input();
      logic();

      // Print the updated game state without clearing the screen
      draw_level_1();

      Sleep(150);
    }
    cout << "Game Over. Your final score is: " << score << endl;
    cin.get();
  }
  else
  {
    while (!gameover)
    {
      // Update the dynamic elements and player input
      POSIXinput();
      logic();

      // Print the updated game state without clearing the screen
      draw_level_1();

      Sleep(150);
    }
    cout << "Game Over. Your final score is: " << score << endl;
    cin.get();
  }
}
void startgame()
{
    savefile_object.open("magian_save.txt", ios::in);
    if (savefile_object.is_open())
    {
      char choice_new_game_variable; 
      cout << "Do you want to start a New Game, or load your existing save?\n"
      "1. New Game\n"
      "2. Continue Game\n"
      "Enter an option (ENTER 0 to exit): ";
      int continue_variable = cin_valid_input();
      switch(continue_variable)
      {
        case 0:
          menu();
          break;
        case 1:
          cout << "Starting new game will Erase old save file. Are you sure? type Y to erase, Or ENTER any other key to Exit: ";
          cin >> choice_new_game_variable;
          if (choice_new_game_variable == 'Y' || choice_new_game_variable == 'y')
          {
            savefile_object.close();
            filesystem::remove("magian_save.txt");
            cout << "Press ENTER to continue.." << endl;
            cin.get();
            newgame();
          }
          else
          {
            menu();
            break;
          }
          break;
        case 2:
          continuegame();
          break;
      }
    }
    else
    {
      newgame();
    }
}
void menu() 
{
  if (music_variable == false)
  {
    PlaySoundW(NULL, NULL, 0);
  }
  else
  {
    PlaySoundW(L"sound//music//Cyber_Attack_by_JuliusH.wav", NULL, SND_FILENAME | SND_ASYNC);
  }
  find_host_os(host_OS_name_variable);
  welcome();
  int menu_variable = cin_valid_input();
  switch (menu_variable)
  {
  case 0:
    exit(1);
  case 1:
    startgame();
    break;
  case 2:
    save_load_game();
    break;
  case 3:
    level_select();
    break;
  case 4:
    help();
    break;
  case 5:
    change_settings(lives, language);
    break;
  case 6:
    library();
    break;
  case 7:
    soundtrack();
    break;
  default:
    cout << "Inccorect option try again";
    menu();
    break;
  }
}
int main()
{
  menu();
  return 0;
}