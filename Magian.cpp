#include <iostream>
// for seed random srand() function
#include <ctime>
// for input() ky presses  _kbhit 
#include <conio.h>
#include <windows.h>
#include <unistd.h>
#include <string>
// for save file read write
#include <fstream>
// for finding save game exists and delete option save_load_game()
#include <filesystem>
// including magian header for saving files functions
#include "save_game.h"
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
  int level_1_enemy_x_pos;
  int level_1_enemy_y_pos;
};
// creating an enemy that damages player by removing 1 life
class enemy_class_2_rakshasa {
public:
  int health=2;
  int speed=2;
  int damage=1;
  int inventory;
  int level_1_enemy_x_pos;
  int level_1_enemy_y_pos;
};
// creating an enemy that damages player by removing 1 life
class enemy_class_2_animated_tree {
public:
  int health;
  int speed;
  int damage;
  int inventory;
  int level_1_enemy_x_pos;
  int level_1_enemy_y_pos;
};
// creating an enemy that damages player by removing 1 life
class enemy_class_3_animated_armour {
public:
  int health;
  int speed;
  int damage;
  int inventory;
  int level_1_enemy_x_pos;
  int level_1_enemy_y_pos;
};
// creating an enemy that damages player by removing 1 life
class enemy_class_3_animated_horror {
public:
  int health;
  int speed;
  int damage;
  int inventory;
  int level_1_enemy_x_pos;
  int level_1_enemy_y_pos;
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
enemy_class_fire level_1_enemy_object;
enemy_class_2_rakshasa level_2_enemy_object;
fstream savefile_object;

// for changing the colour of tiles, NPC's, items etc.,
HANDLE handle_object = GetStdHandle(STD_OUTPUT_HANDLE);

/* Global Variables */
double version = 0.1;
bool gameover = false;
// level 1 width/height
const int width = 20;
const int height = 20;
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
int difficulty=3;
//create 2D array for double buffering
char buffer[height][width];
//changing language variable in settings - future use for include <locale>
int language=1;
// max number of levels
int level=11;
// Extra game mode level select
int level_select_variable=1;

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
// Calling menu function before other void functions so they know it exists at compile time
void menu();
//key bindings to logic()
void check_objective_function();
void check_items_function();
void check_skills_function();
// Function for setting up game before draw()
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
  level_1_enemy_object.level_1_enemy_x_pos = rand() % width;
  level_1_enemy_object.level_1_enemy_y_pos = rand() % height;
  level_1_enemy_object.speed = 1;

}
// Level 1 draw logic
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
      else if (x == level_1_enemy_object.level_1_enemy_x_pos && y == level_1_enemy_object.level_1_enemy_y_pos) {
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

void draw_level_2()
{
    score=0;
    cout << "Level 2 coming soon" << endl;
    menu();
}
void draw_level_3()
{
    score=0;
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

// keyboard WSAD directional movement capture for logic() function
void input() {
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
            cout << "INVENTORY" << endl;
            check_items_function();
            cout << "Press ENTER button to Return to game" << endl;
            cin.get();
            break;
        case 'l':
            cout << "OBJECTIVE" << endl;
            check_objective_function();
            cout << "Press ENTER button to Return to game" << endl;
            cin.get();
            break;
        case 'x':
            cout << "SKILLS" << endl;
            check_skills_function();
            cout << "Press ENTER button to Return to game" << endl;
            cin.get();
            break; 
        case 'q':
            exit(0);
            break;
        }
    }
}
// recieves keyboard input from input() and dictates logic
void logic() {
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
    update_savefile_level();
    draw_level_2();
    }

    // Level 2 - win logic - after specific time searching win condition (friend? or lover) appears
    if (score >= 10) 
    {
    level = 3;
    cout << "You win the level";
    level_select_variable=3;
    update_savefile_level();
    draw_level_3();
    }

   // Level 3 - win logic - specific enemy dies
    if (score >= 20) 
    {
      level = 4;
      cout << "You win the level";
      level_select_variable=4;
      update_savefile_level();
      draw_level_4();
    }



    // Update enemy position randomly
    int random_direction = rand() % 4; // choose a random direction (0 = up, 1 = down, 2 = left, 3 = right)
    switch (random_direction) 
    {
        case 0:
            if (level_1_enemy_object.level_1_enemy_y_pos > 1) 
            {
            level_1_enemy_object.level_1_enemy_y_pos--;
            }
            break;
        case 1:
            if (level_1_enemy_object.level_1_enemy_y_pos < height - 2) 
            {
            level_1_enemy_object.level_1_enemy_y_pos++;
            }
            break;
        case 2:
            if (level_1_enemy_object.level_1_enemy_x_pos > 1) 
            {
            level_1_enemy_object.level_1_enemy_x_pos--;
            }
            break;
        case 3:
            if (level_1_enemy_object.level_1_enemy_x_pos < width - 2) 
            {
            level_1_enemy_object.level_1_enemy_x_pos++;
            }
            break;
    }

  // Check if enemy caught player and update lives if true
  if (x_pos == level_1_enemy_object.level_1_enemy_x_pos && y_pos == level_1_enemy_object.level_1_enemy_y_pos) 
    {
        lives--;
        x_pos = width / 2;
        y_pos = height /2;
    }
}
// starting game funciton called in menu()
void startgame() 
{
  setup();
  lives = difficulty;
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


void check_objective_function()
{
  if(level_select_variable==1) 
  {
    cout << "Collect 10 scrolls to return back to the Magian school to study." << endl;
  }
  if(level_select_variable==2) 
  {
    cout << "Your home is on fire. You should search for survivors." << endl;
  }
  if(level_select_variable==3) 
  {
    cout << "Shikaar 'the hunt' is on for revenge, find the culprit responsible and put an end to their lives" << endl;
  }
  if(level_select_variable==4) 
  {
    cout << "You were captured and taken aboard a foreign ship. You must escape!" << endl;
  }
}

void check_items_function()
{
  cout << "COMING SOON" <<endl;
}

void check_skills_function()
{
  cout << "COMING SOON" <<endl;
}

// Function for greeting Title page
void welcome() 
{
  cout << "\n\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~    MAGIAN    ~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n\n" 
  "A game of mages from the school of Agni in Kashmir mountain basin. \n"
  "Collect from scrolls, herbs, gold and items to build your player and advance through the level. \n"
  "but dont fall off the path less you die! \n\n\n"
  "MAIN MENU\n"
  "1. Start Game\n"
  "2. Save & Load\n"
  "3. Level select\n"
  "4. Help\n"
  "5. Settings\n"
  "6. Exit\n\n"
  "Enter an option: " << endl;
}
// Bonus game mode - select any level unlocked from savefile.txt
void level_select_function()
{
  match_savefile_level_function();
  
  cout << "Choose any level you've unlocked \n"; 

  // add a condition to show only levels that are below or equal to level_select
  for (int i = 1; i <= level_select_variable; i++) {
    switch (i) {
      case 1:
        cout << "1. Kashmir_Basin\n";
        break;
      case 2:
        cout << "2. Agni_Mandir_ablaze\n";
        break;
      case 3:
        cout << "3. Shikaar\n";
        break;
      case 4:
        cout << "4. Seized_aboard\n";
        break;
      case 5:
        cout << "5. Capsize\n";
        break;
      case 6:
        cout << "6. Underwater_hell\n";
        break;
      case 7:
        cout << "7. Final_spell\n";
        break;
      case 8:
        cout << "8. Rakashaa_ghar\n";
        break;
      case 9:
        cout << "9. नरक_Hell\n";
        break;
      case 10:
        cout << "10. संसार_Samsara\n";
        break;
      case 11:
        cout << "11. यम_Yama\n";
        break;
      default:
        break;
    }
  }

  // ask the user for their choice
  int level_select_choice_variable;
  cout << "Enter your choice (Press 0 to exit): ";
  cin >> level_select_choice_variable;

  // execute the selected option
  switch (level_select_choice_variable)
  {
    case 0:
      menu();
      break;
    case 1:
      PlaySoundW(L"sound//music//alien-jungle.wav", NULL, SND_FILENAME | SND_ASYNC);
      setup();
      lives = difficulty;
      while (!gameover) 
      {
        draw_level_1();
        input();
        logic();
        Sleep(150);
      }
      cout << "Game Over. Your final score is: " << score << endl;
      cin.get();
      break;
    case 2:
      PlaySoundW(L"sound//music//Shima-uta_seige.wav", NULL, SND_FILENAME | SND_ASYNC);
      setup();
      lives = difficulty;
      while (!gameover) 
      {
        draw_level_2();
        input();
        logic();
        Sleep(150);
      }
      cout << "Game Over. Your final score is: " << score << endl;
      cin.get();
      break;
      break;
    case 3:
      draw_level_3();
      break;
    case 4:
      draw_level_4();
      break;
    case 5:
      draw_level_5();
      break;
    case 6:
      draw_level_6();
      break;
    case 7:
      draw_level_7();
      break;
    case 8:
      draw_level_8();
      break;
    case 9:
      draw_level_9();
      break;
    case 10:
      draw_level_10();
      break;
    case 11:
      draw_level_11();
      break;
    default:
      break;
  }
}
// Function for help on how to play game
void help() 
{
  cout << "MAGIAN.\n\nA game of mages from the school of Agni in Kashmir mountain basin. \n"
  "collect from scrolls, herbs, gold and items to build your player\n and advance through the level.\n\n" << endl;
  cout << "The build version for this software is: " << version << endl;

  cout << "\n\nHOW TO PLAY\n\n"
  "This character represents you स - This is you, stay safe and collect objectives.\n"
  "Scrolls look like this ₹ - If you find one you get a point \n"
  "Enemies look like this क - If you touch them you lose a life \n"
  "Walls look like this # - If you touch them you lose a life \n"
  "\n\nBUTTONS\n\n"
  "To move use the keyboard buttons to walk 'W' = UP,'S' = Down,'A' = Left,'D' = RIGHT \n"
  "Q = quit game.\n"
  "L = Check level objective to see goals to complete level. for example collect 10 scrolls.\n"
  "I = Check inventory and use items you found.\n"
  "X = Check and use skills you gained.\n"
  "\n\nOBJECTIVE\n\n"
  "Make it to the end of the levels. Each has their own goals for example, collect 10 scrolls \n"
  "or find the exit without getting killed." << endl;
}
// Function for language setup in the future
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
// Function for changing in game settings
void settings() 
{
  cout << "SETTINGS\n\n"
  "DIFFICULTY\n"
  "Choose a difficulty level\n"
  "1. Hard - 1 Life \n"
  "2. Medium - 2 Life \n"
  "3. Easy - 3 Life \n\n"
  "LANGUAGE\n"
  "4. Change language\n\n"
  "MAIN MENU\n"
  "5. Exit back to Main menu \n\n"
  "Enter an option: ";
  cin >> difficulty;
  switch (difficulty) {
  case 1:
    lives = 1;
    break;
  case 2:
    lives = 2;
    break;
  case 3:
    lives = 1;
    break;
  case 4:
    cout << "CHANGE LANGUAGE coming soon" << endl;
    break;
  case 5:
    menu();
    return;
  default:
    cout << "Incorrect choice, try again." << endl;
    settings();
    break;
  }
}





// Function for in game menu
void menu() 
{
  PlaySoundW(L"sound//music//Cyber_Attack_by_JuliusH.wav", NULL, SND_FILENAME | SND_ASYNC);
  welcome();
  cin >> menu_variable;
  switch (menu_variable) 
  {
  case 1:
  PlaySoundW(L"sound//music//alien-jungle.wav", NULL, SND_FILENAME | SND_ASYNC);
    startgame();
    break;
  case 2:
    save_load_game();
    break;
  case 3:
    level_select_function();
    break;
  case 4:
    help();
    menu();
    break;
  case 5:
    settings();
    menu();
    break;
  case 6:
    cout << "Goodbye" << endl;
    exit(1);
    break;
  }
}
// Int main
int main() 
{
  menu();
  return 0;
}