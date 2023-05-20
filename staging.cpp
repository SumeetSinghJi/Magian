// Windows OS specific headers below
// In input() the command _kbhit uses user input to move key
#include <conio.h>
#include <windows.h>
// POSIX (Unix) based (Linux/MacOS/etc.) OS specific headers below
#include <unistd.h>
// for seed random srand() function
#include <ctime>
#include <iostream>
#include <string>
// including vector header for inventory
#include <vector>
// for smart pointers
#include <memory>
// for save file read write
#include <fstream>
// for finding save game exists and delete option save_load_game()
#include <filesystem>
// including magian header for saving files functions
#include "save_game.h"
// settings header contains main menu settings options
#include <game_settings.h>
// main menu level select option is in own header
#include <level_select.h>
// check objectives skill
#include <get_objective.h>
using namespace std;
// Global classes
// creating an enemy that damages player by removing 1 life
class enemy_class 
{
public:
  string enemy_name;
  string enemy_description;
  int damage;
  int health;
  int resistance_fire;
  int resistance_electricity;
  int resistance_water;
  int resistance_earth;
  int defence;
  int speed;
  int enemy_x_pos;
  int enemy_y_pos;
};
// Item class
class item_class 
{
public:
  string name;
  string description;
  int cost;
  int effect;
  item_class(string name, string description)
  {
    this->name = name;
    this->description = description;
  }
  virtual void use(int& value)
  {
    /*
    This section deliberately left blank
    This is Virtual function that makes this item_class polymorphic.
    This virtual function will be overwritten by the subclass e.g. potion_item_subclass

    the use function for subclasses will be used in check_item() index dynamic cast
    use for that item for that subclass items effect
    */
  }
};
class potion_item_subclass : public item_class
{
public:
  potion_item_subclass() : item_class("Potion", "When used increases players life by 1 by regenerating their body.") {}
  void use(int& lives)
  {
    cout << "You used a Potion. 1 life gained" << endl;
    lives++;
  }
};
class leather_boots_item_subclass : public item_class
{
public:
  leather_boots_item_subclass() : item_class("Leather boots", "When used increases players speed by 1 by cushioning their steps.") {}
  void use(int& player_speed)
  {
    cout << "You wore leather boots. Speed permanently increased by 1" << endl;
    player_speed++;
  }
};
// creating an enemy that damages player by removing 1 life
/* Global Objects */
enemy_class level_1_enemy_object;
fstream savefile_object;
/* Global Enum*/
enum edirection 
{
  STOP = 0, UP, DOWN, LEFT, RIGHT
};
edirection direction;
/* Global Vectors*/
vector<unique_ptr<item_class>> inventory_vector;
/* Global Variables */
double version = 0.2;
bool gameover = false;
// level 1 width/height
const int width = 20;
const int height = 20;
int moneyx, moneyy;
int score = 0;
int money = 0;
int x_pos, y_pos;
int lives = 3;
int difficulty=3;
// language = 1 is English by default
int language=1;
int player_speed=1;
//create 2D array for double buffering
char buffer[height][width];
// max number of levels
int level=11;
// Extra game mode level select
int level_select_variable=1;
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
// Function prototype Calling menu function before other void functions so they know it exists at compile time
void menu();
void check_items();
void check_skills();
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
  level_1_enemy_object.enemy_name="Fire";
  level_1_enemy_object.enemy_description="A large moving flame 2 meters high burning everything it touches.";
  level_1_enemy_object.enemy_x_pos = rand() % width;
  level_1_enemy_object.enemy_y_pos = rand() % height;

  // to refresh inventory for level select
  inventory_vector.clear();
  

  // Adding starting items to players inventory vector
  inventory_vector.push_back(make_unique<potion_item_subclass>());
  inventory_vector.push_back(make_unique<leather_boots_item_subclass>());

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
  clear_screen();
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
        case 'q':
            exit(0);
            break;
        }
    }
}
void input_POSIX()
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
        case 'q':
            exit(0);
            break;
        }
    }
}
// recieves keyboard input from input() and dictates logic
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
void startgame_POSIX() 
{
  setup();
  while (!gameover) 
  {
    draw_level_1();
    input_POSIX();
    logic();
    Sleep(150);
  }
  cout << "Game Over. Your final score is: " << score << endl;
  cin.get();
}
// starting game funciton called in menu()
void game_os_check()
{
    if(find_host_os()=="Windows")
    {
        startgame();
    }
    else
    {
        startgame_POSIX();
    }
}
void check_items()
{
  
  cout << "INVENTORY" <<endl;
  for (int i=0;i<inventory_vector.size(); i++)
  {
    cout << i << ": " << inventory_vector[i]->name << endl;
  }

  // using an item
  int item_select_variable;
  cout << "Select item you wish to use: " << endl;
  cin >> item_select_variable;

  if (item_select_variable >= 0 && item_select_variable < inventory_vector.size())
  {
    cout << "Using item: " << inventory_vector[item_select_variable]->name << endl;
    unique_ptr<item_class>& item = inventory_vector[item_select_variable];
    if (inventory_vector[item_select_variable]->name == "Potion")
    {
      potion_item_subclass* potion = dynamic_cast<potion_item_subclass*>(item.get());
      potion->use(lives);
    }
    else if (inventory_vector[item_select_variable]->name == "Leather boots")
    {
      leather_boots_item_subclass* boots = dynamic_cast<leather_boots_item_subclass*>(item.get());
      boots->use(player_speed);
    }

    inventory_vector.erase(inventory_vector.begin() + item_select_variable);
  }
  else
  {
    cout << "Invalid item index option" << endl;
  }
}
void check_skills()
{
  cout << "COMING SOON" <<endl;
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
      PlaySoundW(L"sound//music//Cyber_Attack_by_JuliusH.wav", NULL, SND_FILENAME | SND_ASYNC);
      cin.get();
      break;
    case 2:
      PlaySoundW(L"sound//music//alien-jungle.wav", NULL, SND_FILENAME | SND_ASYNC);
      cin.get();
      break;
    case 3:
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
// Function for greeting Title page
void welcome() 
{ 
  cout << "\n\n"
  "      ▄▄▄▄███▄▄▄▄      ▄████████    ▄██████▄   ▄█     ▄████████ ███▄▄▄▄ \n" 
  "   ▄██▀▀▀███▀▀▀██▄   ███    ███   ███    ███ ███    ███    ███ ███▀▀▀██▄\n" 
  "   ███   ███   ███   ███    ███   ███    █▀  ███▌   ███    ███ ███   ███\n" 
  "   ███   ███   ███   ███    ███  ▄███        ███▌   ███    ███ ███   ███\n" 
  "   ███   ███   ███ ▀███████████ ▀▀███ ████▄  ███▌ ▀███████████ ███   ███\n" 
  "   ███   ███   ███   ███    ███   ███    ███ ███    ███    ███ ███   ███\n" 
  "   ███   ███   ███   ███    ███   ███    ███ ███    ███    ███ ███   ███\n" 
  "     █   ███   █▀    ███    █▀    ████████▀  █▀     ███    █▀   ▀█   █▀ \n"                                                                        
  "  \n\n\n" 
  "A game of mages from the school of Agni in Kashmir mountain basin. \n"
  "Collect from scrolls, herbs, gold and items to build your player\nand advance through the level. \n"
  "but dont fall off the path less you die! \n\n\n"
  "MAIN MENU\n"
  "1. Start Game\n"
  "2. Save & Load\n"
  "3. Level select\n"
  "4. Help\n"
  "5. Settings\n"
  "6. Soundtrack\n\n"
  "Choose an option (ENTER 0 to exit): ";
}
// Function for help on how to play game
void help() 
{
  cout << "\n\n" 
  "                         ░░░░░░░░░░░░░░░▄▀▄░░░░░░░░░░░░░░░\n"
  "                         ░░░░░░░░░░░░░▄▀░░░▀▄░░░░░░░░░░░░░\n"
  "                         ░░░░░░░░░░░▄▀░░░░▄▀█░░░░░░░░░░░░░\n"
  "                         ░░░░░░░░░▄▀░░░░▄▀░▄▀░▄▀▄░░░░░░░░░\n"
  "                         ░░░░░░░▄▀░░░░▄▀░▄▀░▄▀░░░▀▄░░░░░░░\n"
  "                         ░░░░░░░█▀▄░░░░▀█░▄▀░░░░░░░▀▄░░░░░\n"
  "                         ░░░▄▀▄░▀▄░▀▄░░░░▀░░░░▄█▄░░░░▀▄░░░\n"
  "                         ░▄▀░░░▀▄░▀▄░▀▄░░░░░▄▀░█░▀▄░░░░▀▄░\n"
  "                         ░█▀▄░░░░▀▄░█▀░░░░░░░▀█░▀▄░▀▄░▄▀█░\n"
  "                         ░▀▄░▀▄░░░░▀░░░░▄█▄░░░░▀▄░▀▄░█░▄▀░\n"
  "                         ░░░▀▄░▀▄░░░░░▄▀░█░▀▄░░░░▀▄░▀█▀░░░\n"
  "                         ░░░░░▀▄░▀▄░▄▀░▄▀░█▀░░░░▄▀█░░░░░░░\n"
  "                         ░░░░░░░▀▄░█░▄▀░▄▀░░░░▄▀░▄▀░░░░░░░\n"
  "                         ░░░░░░░░░▀█▀░▄▀░░░░▄▀░▄▀░░░░░░░░░\n"
  "                         ░░░░░░░░░░░░░█▀▄░▄▀░▄▀░░░░░░░░░░░\n"
  "                         ░░░░░░░░░░░░░▀▄░█░▄▀░░░░░░░░░░░░░\n"
  "                         ░░░░░░░░░░░░░░░▀█▀░░░░░░░░░░░░░░░\n\n\n"
  "DISCLAIMER: This game and the symbols, language, and references used represent the culture of\n"
  "the Indo-Iranian people known collectivly as Aryans. We do not condone any violence or hate.\n\n\n"
  "A game of mages from the school of Agni in Kashmir mountain basin. \n"
  "collect from scrolls, herbs, gold and items to build your player\nand advance through the level.\n\n" << endl;
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
  "or find the exit without getting killed.\n"<< endl;
  cout << "Press Enter to continue...";
  cin.ignore();
  cin.get();
  menu();
}
// Function for in game menu
void menu() 
{
  PlaySoundW(L"sound//music//Cyber_Attack_by_JuliusH.wav", NULL, SND_FILENAME | SND_ASYNC);
  welcome();
  int menu_variable = cin_valid_input();
  switch (menu_variable) 
  {
  case 0:
    exit(1);
  case 1:
  PlaySoundW(L"sound//music//alien-jungle.wav", NULL, SND_FILENAME | SND_ASYNC);
    game_os_check();
    menu();
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
    soundtrack();
    break;
  default:
    cout << "Inccorect option try again";
    menu();
    break;
  }
}
// Int main
int main() 
{
  menu();
  return 0;
}