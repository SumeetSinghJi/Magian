#include <conio.h>
#include <windows.h>
#include <unistd.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
using namespace std;
class enemy_class 
{
public:
  string enemy_name;
  string enemy_description;
  int damage;
  int health;
  int speed;
  int enemy_x_pos;
  int enemy_y_pos;
};
enemy_class level_1_enemy_object;
enum edirection 
{
  STOP = 0, UP, DOWN, LEFT, RIGHT
};
edirection direction;
double version = 0.2;
bool gameover = false;
const int width = 20;
const int height = 20;
int moneyx, moneyy;
int score = 0;
int money = 0;
int x_pos, y_pos;
int lives = 3;
int difficulty=3;
int language=1;
int player_speed=1;
char buffer[height][width];
int level=11;
int level_select_variable=1;
void menu();
void setup() 
{
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

  // level 1 setup enemy
  level_1_enemy_object.enemy_name="Fire";
  level_1_enemy_object.enemy_description="A large moving flame 2 meters high burning everything it touches.";
  level_1_enemy_object.enemy_x_pos = rand() % width;
  level_1_enemy_object.enemy_y_pos = rand() % height;

}
void draw_level_1() {
  // Draw top wall  
  for (int top_wall = 0; top_wall < width; top_wall++) {
    buffer[0][top_wall] = '#';
  }

  // draw middle section
  //loop through y axis 19 times down
  for (int y = 1; y < height-1; y++) {
    // loop through x axis 19 times across
    for (int x = 0; x < width; x++) {
      // draw side wall
      if (x == 0 || x == width - 1) {
        buffer[y][x] = '#';
      }
      //draw player
      else if (x == x_pos && y == y_pos) {
        buffer[y][x] = 'P';
      }
      //draw money
      else if (x == moneyx && y == moneyy) {
        buffer[y][x] = '$';
      } 
      //draw enemy
      else if (x == level_1_enemy_object.enemy_x_pos && y == level_1_enemy_object.enemy_y_pos) {
        buffer[y][x] = 'E';
      }    
      //draw blank if not wall or player
      else {
        buffer[y][x] = ' ';
      }
    }
  }

  // Draw bottom wall
  for (int bottom_wall = 0; bottom_wall < width; bottom_wall++) {
    buffer[height-1][bottom_wall] = '#';
  }

  // print the current score and lives beneath the array
  string score_str = "Current Score: " + to_string(score);
  string lives_str = "Current Lives: " + to_string(lives);
  int score_pos = (width - score_str.length()) / 2;
  int lives_pos = (width - lives_str.length()) / 2;

  for (int i = 0; i < score_str.length(); i++) {
    buffer[height][score_pos+i] = score_str[i];
  }
  for (int i = 0; i < lives_str.length(); i++) {
    buffer[height+1][lives_pos+i] = lives_str[i];
  }
  system("cls");
  for (int y = 0; y < height+2; y++) {
    for (int x = 0; x < width; x++) {
      cout << buffer[y][x];
    }
    cout << endl;
  }
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
        case 'q':
            exit(0);
            break;
        }
    }
}
void logic() 
{
    // Update player position based on direction
    switch (direction) 
    {
        case UP:
            y_pos--;
            break;
        case DOWN:
            y_pos++;
            break;
        case LEFT:
            x_pos--;
            break;
        case RIGHT:
            x_pos++;
            break;
    }

    // Check if player hit a wall and reset position to center if true
    if (x_pos == 0 || x_pos == width - 1 || y_pos == 0 || y_pos == height - 1) 
    {
        lives--;
        x_pos = width / 2;
        y_pos = height /2;
    }

    

    // Check if player has run out of lives and end the game if true
    if (lives == 0) 
    {
        cout << "You died!" << endl;
        menu();
    }

    // Level 1 score - Check if player picked up money and update score and money location if true
    if (x_pos == moneyx && y_pos == moneyy) 
    {
        score++;
        money++;
        moneyx = rand() % width-1;
        moneyy = rand() % height-1;
    }


    // Level 1 - win logic 
    if (score >= 1) 
    {
    level = 2;
    cout << "You win the level";
    level_select_variable=2;
    }


    // Update enemy position randomly
    int random_direction = rand() % 4; // choose a random direction (0 = up, 1 = down, 2 = left, 3 = right)
    switch (random_direction) 
    {
        case 0:
            if (level_1_enemy_object.enemy_y_pos > 1) 
            {
            level_1_enemy_object.enemy_y_pos--;
            }
            break;
        case 1:
            if (level_1_enemy_object.enemy_y_pos < height - 2) 
            {
            level_1_enemy_object.enemy_y_pos++;
            }
            break;
        case 2:
            if (level_1_enemy_object.enemy_x_pos > 1) 
            {
            level_1_enemy_object.enemy_x_pos--;
            }
            break;
        case 3:
            if (level_1_enemy_object.enemy_x_pos < width - 2) 
            {
            level_1_enemy_object.enemy_x_pos++;
            }
            break;
    }
  // Check if enemy caught player and update lives if true
  if (x_pos == level_1_enemy_object.enemy_x_pos && y_pos == level_1_enemy_object.enemy_y_pos) 
    {
        lives--;
        x_pos = width / 2;
        y_pos = height /2;
    }



}
void shoot(int width, int height, int x, int y, int direction)
{
  // Calculate the target position based on the direction
  int targetX = x;
  int targetY = y;

  if (direction == 0) {
    // Up
    targetY--;
  } else if (direction == 1) {
    // Up-right
    targetX++;
    targetY--;
  } else if (direction == 2) {
    // Right
    targetX++;
  } else if (direction == 3) {
    // Down-right
    targetX++;
    targetY++;
  } else if (direction == 4) {
    // Down
    targetY++;
  } else if (direction == 5) {
    // Down-left
    targetX--;
    targetY++;
  } else if (direction == 6) {
    // Left
    targetX--;
  } else if (direction == 7) {
    // Up-left
    targetX--;
    targetY--;
  }

  // Check if the target position is within the bounds of the game map
  if (targetX >= 0 && targetX < width && targetY >= 0 && targetY < height)
  {
    // Check if there is a target at the target position
    if (buffer[targetX][targetY] == 'E')
    {
      // Remove the target by setting the corresponding element in the buffer to an empty space
      buffer[targetX][targetY] = ' ';
    }
  }
}
void startgame() 
{
  setup();
  while (!gameover) 
  {
    draw_level_1();
    input();
    logic();
    Sleep(150);
  }
  cout << "Game Over. Your final score is: " << score << endl;
  cin.get();
}
void welcome() 
{ 
  cout << "\n\n"
  "A game of mages from the school of Agni in Arianna. \n"
  "MAIN MENU\n"
  "1. Start Game\n"
  "Choose an option (ENTER 0 to exit): ";
}
void menu() 
{
  welcome();
  int menu_variable;
  cin >> menu_variable;
  switch (menu_variable) 
  {
  case 0:
    exit(1);
  case 1:
    startgame();
    menu();
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