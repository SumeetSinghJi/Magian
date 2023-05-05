#include <iostream>
#include <ctime>
#include <conio.h>
#include <windows.h>
#include <unistd.h>
#include <string>
#include <fstream>
//custom developed level select header included with source files
#include <level_select.h>
using namespace std;

// Global Macros
// to check if ncurses.h header exists and if not ignore all further code that relies on it e.g. clear screen
#ifdef HAVE_NCURSES_H
#include <ncurses.h>
#endif


// Global classes
// creating an enemy that damages player by removing 1 life
class enemy_class_fire {
public:
  int health=2;
  int speed=1;
  int damage;
  int inventory;
  int first_enemy_x_pos;
  int first_enemy_y_pos;
};
// creating an enemy that damages player by removing 1 life
class enemy_class_2_rakshasa {
public:
  int health=2;
  int speed=2;
  int damage=1;
  int inventory;
  int first_enemy_x_pos;
  int first_enemy_y_pos;
};
// creating an enemy that damages player by removing 1 life
class enemy_class_2_animated_tree {
public:
  int health;
  int speed;
  int damage;
  int inventory;
  int first_enemy_x_pos;
  int first_enemy_y_pos;
};
// creating an enemy that damages player by removing 1 life
class enemy_class_3_animated_armour {
public:
  int health;
  int speed;
  int damage;
  int inventory;
  int first_enemy_x_pos;
  int first_enemy_y_pos;
};
// creating an enemy that damages player by removing 1 life
class enemy_class_3_animated_horror {
public:
  int health;
  int speed;
  int damage;
  int inventory;
  int first_enemy_x_pos;
  int first_enemy_y_pos;
};
/*
class magic_stop_time_class {
  public:
    void magic_stop_time_class_method() {
      
    }
}
Also include void to cast and add to key binding or possibly logic..
*/


/* Global Objects */
enemy_class_fire enemy_object;
enemy_class_2_rakshasa level_2_enemy_object;

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
int level_select_counter=1;



//function prototype

void clear_screen()
{
#ifdef HAVE_NCURSES_H
  //initialises ncurses header
  initscr();
  // clears screen
  clear();
  //refreshs the screen
  refresh();
  // cleans up ncurses
  endwin();
#else
  //use the default system command below. if Windows do "cls", if unix do "clear"
#ifdef WIN_32
  system("cls");
#else
  system("clear");
#endif
#endif
}

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

  // level 2 setup enemy
  level_2_enemy_object.first_enemy_x_pos = rand() % level_2_width;
  level_2_enemy_object.first_enemy_y_pos = rand() % level_2_height;
  level_2_enemy_object.speed = 1;
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

// Level 2 draw logic
void draw_level_2() {

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
void logic() {
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

  // Level 2 logic - Check if player hit a wall and reset position to center if true
  if (x_pos == 0 || x_pos == level_2_width - 1 || y_pos == 0 || y_pos == level_2_height - 1) {
    lives--;
    x_pos = level_2_width / 2;
    y_pos = level_2_height /2;
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

   // Level 2 score - Check if player picked up money and update score and money location if true
  if (x_pos == moneyx && y_pos == moneyy) {
    score++;
    money++;
    moneyx = rand() % level_2_width-1;
    moneyy = rand() % level_2_height-1;
  }

  // Level 1 - win logic 
  if (score > 10) 
  {
  level = 2;
  cout << "You win the level";
  draw_level_2();
  level_select_counter=1;
  
  }

   // Level 2 - win logic - after specific time searching win condition (friend? or lover) appears
  if (score > 40) 
  {
  level = 3;
  cout << "You win the level";
  //draw_level_3();
  level_select_counter=2;

  }

   // Level 3 - win logic - specific enemy dies
  if (score > 80) 
  {
  level = 4;
  cout << "You win the level";
  //draw_level_4();
  level_select_counter=3;
  }



  // Update enemy position randomly
  int random_direction = rand() % 4; // choose a random direction (0 = up, 1 = down, 2 = left, 3 = right)
  switch (random_direction) {
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
  cout << "Welcome to Magian \n" 
  "A game of mages from the school of Agni in Kashmir mountain basin. \n"
  "Collect from scrolls, herbs, gold and items to build your player and advance through the level. \n"
  "but dont fall off the path less you die! \n\n\n"
  "Main Menu \n"
  "1. Start Game\n"
  "2. Help \n"
  "3. Settings \n"
  "4. Bonus - Level Select \n"
  "5. Exit \n\n"
  "Enter an option: " << endl;
}


string update() 
{
  time_t now = time(0);
    tm* ltm = localtime(&now);
    int year = 1900 + ltm->tm_year;
    int month = 1 + ltm->tm_mon;
    int day = ltm->tm_mday;
    string update_variable = to_string(day) + "/" + to_string(month) + "/" + to_string(year);
    return update_variable;
}

void help() 
{
  cout << "Magian. A game of mages from the school of Agni in Kashmir mountain basin. \n"
  "collect from scrolls, herbs, gold and items to build your player and advance through the level.\n" << endl;
  cout << "The build version for this software is: " << version << endl;

  cout << "HOW TO PLAY\n\n"
  "This character represents you स - This is you, stay safe and collect objectives.\n"
  "To move use the keyboard buttons to walk 'W' = UP,'S' = Down,'A' = Left,'D' = RIGHT \n"
  "Scrolls look like this ₹ - If you find one you get a point \n"
  "Enemies look like this क - If you touch them you lose a life \n"
  "Walls look like this # - If you touch them you lose a life \n"
  "OBJECTIVE. \n\n"
  "Make it to the end of the levels. Each has their own goals for example, collect 10 scrolls \n"
  "or find the exit without getting killed." << endl;
}

/*
void language() 
{
  
  cout << "choose your language \n\n"
  "English \n"
  "Japanese \n"
  "Hindustani \n" << endl;
  switch(language) 
  {
    case 1:
      language=1;
      break;
    case 2:
      language=2;
      break;
    case 3:
      language=3;
      break;
    default 1:
      cout << "Incorrect option." << endl;
      return;
  }
  if(language == 1) 
  {

  }
  else if(language ==2) 
  {

  }
  else if(language==3)
  {

  }
}
*/

void settings() 
{
  cout << "Choose a difficulty level \n"
  "1. Easy - 3 Lives \n"
  "2. Medium - 2 Lives \n"
  "3. Hard - 1 Life \n\n\n\n\n"
  "4. Change language"
  "5. Exit back to Main menu \n"
  "Enter an option: ";
  cin >> difficulty;
  switch (difficulty) {
  case 1:
    lives = 3;
    break;
  case 2:
    lives = 2;
    break;
  case 3:
    lives = 1;
    break;
  case 4:
    cout << "Feature coming soon" << endl;
    break;
  case 5:
    menu();
    return;
  default:
    cout << "Incorrect choice, try again." << endl;
    settings();
    break;
  }

  menu();
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
    help();
    menu();
    break;
  case 3:
    settings();
    menu();
    break;
  case 4:
    level_select();
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