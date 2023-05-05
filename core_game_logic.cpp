#include <iostream>
#include <ctime>
#include <conio.h>
#include <windows.h>
#include <unistd.h>
#include <string>
#include <fstream>
using namespace std;


// Global classes
// creating an enemy that damages player by removing 1 life
class enemy_class {
public:
  int speed;
  int first_enemy_x_pos;
  int first_enemy_y_pos;
};


/* Global Objects */
enemy_class enemy_object;


// for changing the colour of tiles, NPC's, items etc.,
HANDLE handle_object = GetStdHandle(STD_OUTPUT_HANDLE);

/* Global Variables */
double version = 0.1;
bool gameover = false;
// level 1 width/height
const int width = 20;
const int height = 20;
// level 2 width/height
const int level_2_width = 60;
const int level_2_height = 60;
int moneyx, moneyy;
int score = 0;
int money = 0;
int inventory[5];
int x_pos, y_pos;
enum edirection {
  STOP = 0, UP, DOWN, LEFT, RIGHT
};
edirection direction;
int menu_variable;
int lives = 3;
int difficulty;
//create 2D array for double buffering
char buffer[height][width];
//changing language variable in settings - future use for include <locale>
int language=1;
// max number of levels
int level=11;
// Extra game mode level select
int level_select_variable=1;





void menu();

void setup() {

  // Below srand method needs to be executed at runtime hence run in a function vs global variable;
  moneyx = rand() % width;
  moneyy = rand() % height;

  // setting position of player
  direction = STOP;
  x_pos = width / 2;
  y_pos = height / 2;

  // level 1 setup enemy
  enemy_object.first_enemy_x_pos = rand() % width;
  enemy_object.first_enemy_y_pos = rand() % height;
  enemy_object.speed = 1;

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
      else if (x == enemy_object.first_enemy_x_pos && y == enemy_object.first_enemy_y_pos) {
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

  // clear the console and print the buffer
  system("cls");
  for (int y = 0; y < height+2; y++) {
    for (int x = 0; x < width; x++) {
      cout << buffer[y][x];
    }
    cout << endl;
  }
}



// keyboard WSAD directional movement capture for logic() function
void input() {
  if (_kbhit()) {
    switch (_getch()) {
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
    }
  }
}

// recieves keyboard input from input() and dictates logic
void logic() 
{
  // Update player position based on direction
  switch (direction) {
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
  if (x_pos == 0 || x_pos == width - 1 || y_pos == 0 || y_pos == height - 1) {
    lives--;
    x_pos = width / 2;
    y_pos = height /2;
  }


  // Check if player has run out of lives and end the game if true
  if (lives == 0) {
    cout << "You died!" << endl;
    menu();
  }

  // Level 1 score - Check if player picked up money and update score and money location if true
  if (x_pos == moneyx && y_pos == moneyy) {
    score++;
    money++;
    moneyx = rand() % width-1;
    moneyy = rand() % height-1;
  }


  // Level 1 - win logic 
  if (score > 3) 
  {
  level = 2;
  cout << "You win the level";
  gameover=TRUE;
  level_select_variable=1;

  }
  


  // Update enemy position randomly
  int random_direction = rand() % 4; // choose a random direction (0 = up, 1 = down, 2 = left, 3 = right)
  switch (random_direction) 
  {
    case 0:
      if (enemy_object.first_enemy_y_pos > 1) {
        enemy_object.first_enemy_y_pos--;
      }
      break;
    case 1:
      if (enemy_object.first_enemy_y_pos < height - 2) {
        enemy_object.first_enemy_y_pos++;
      }
      break;
    case 2:
      if (enemy_object.first_enemy_x_pos > 1) {
        enemy_object.first_enemy_x_pos--;
      }
      break;
    case 3:
      if (enemy_object.first_enemy_x_pos < width - 2) {
        enemy_object.first_enemy_x_pos++;
      }
      break;
  }

  // Check if enemy caught player and update lives if true
  if (x_pos == enemy_object.first_enemy_x_pos && y_pos == enemy_object.first_enemy_y_pos) {
    lives--;
    x_pos = width / 2;
    y_pos = height /2;
  }

}




// starting game funciton called in menu()

void startgame() {
  setup();
  // reset the gameover flag to false
  gameover = false;
  while (!gameover) {
    draw_level_1();
    input();
    logic();
    Sleep(150);
  }
  cout << "Game Over. Your final score is: " << score << endl;
  system("pause");
}



void menu();


void welcome() 
{
  cout << "Welcome to Magian" << endl;
  cout << "Press the number 1 to begin." << endl;
}





void menu() 
{

  welcome();
  cin >> menu_variable;
  switch (menu_variable) 
  {
  case 1:
    startgame();
    break;
  case 2:
    menu();
    break;
  case 3:
    menu();
    break;
  case 4:
    break;
  case 5:
    cout << "Goodbye" << endl;
    exit(1);
    break;
  }
}

int main() 
{
  menu();
  return 0;
}