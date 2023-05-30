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
#include <headers/save_game.h>
using namespace std;
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
fstream savefile_object;
enum edirection 
{
  STOP = 0, UP, DOWN, LEFT, RIGHT
};
edirection direction;
vector<unique_ptr<item_class>> inventory_vector;
string version = "0.2.1";
bool gameover = false;
const int width = 20; // size of map
const int height = 20; // size of map
int moneyx, moneyy; // money draw() position
int score = 0;
int money = 0;
int x_pos, y_pos; // player draw() position
int lives = 3;
int difficulty=3;
int language=1; // language 1 = english
int player_speed=1;
char buffer[height][width]; // draw level 1 width/height
int level=11;
int level_select_variable=1; // for bonus level select
clock_t lastShootTime; // shoot() time management variables
const int shootInterval = 1000; // 1 second in milliseconds
void menu();
void check_items();
void item_store(vector<unique_ptr<item_class>>& inventory_vector);
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

  // to refresh vectors for new game and level select
  inventory_vector.clear();

  // Adding starting items to players inventory vector
  inventory_vector.push_back(make_unique<potion_item_subclass>());
  item_store(inventory_vector);
  inventory_vector.push_back(make_unique<leather_boots_item_subclass>());
  item_store(inventory_vector);

  //initialise buffer with default character ' ' (space) to avoid console buffer not clearing.
  for (int i = 0; i < height; i++)
  {  
    for (int j = 0; j < width; j++)
    {
        buffer[i][j] = ' ';
    }
  }
}
// Saving picked up items into game save file
void item_store(vector<unique_ptr<item_class>>& inventory_vector)
{
    // Accessing last Element of inventory vector
    int inventory_last_index_variable = inventory_vector.size() - 1;
    // accessing last item
    item_class* inventory_last_element_variable = inventory_vector[inventory_last_index_variable].get();
    string item_store_variable = inventory_last_element_variable->name;
    // Storing last item to savegame file
    savefile_object.open("magian_save.txt", ios::app);
    if(savefile_object.is_open())
    {
        savefile_object << item_store_variable << endl;
        savefile_object.close();
    }
    else
    {
        cerr << "Error: Inventory not saved to save file" << endl;
        return;
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
        // Initialize to empty space
        buffer[y][x] = ' ';
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
        case 'q':
            exit(0);
            break;
        case 'e':
            direction = STOP;
            break;
        }
    }
}
void logic() 
{
  // Calculate the proposed new position based on direction
  int new_x_pos = x_pos;
  int new_y_pos = y_pos;
  switch (direction) 
  {
    case UP:
        new_y_pos--;
        break;
    case DOWN:
        new_y_pos++;
        break;
    case LEFT:
        new_x_pos--;
        break;
    case RIGHT:
        new_x_pos++;
        break;
  }

  // Check if the new player position is within the bounds of the game map
  if (new_x_pos >= 1 && new_x_pos < width - 1 && new_y_pos >= 1 && new_y_pos < height - 1) 
  {
    // If the new position is within the bounds, update player's position
    x_pos = new_x_pos;
    y_pos = new_y_pos;
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
      inventory_vector.erase(inventory_vector.begin() + item_select_variable);
      cout << "Press ENTER to continue...";
      cin.get();
    }
    else if (inventory_vector[item_select_variable]->name == "Leather boots")
    {
      leather_boots_item_subclass* boots = dynamic_cast<leather_boots_item_subclass*>(item.get());
      boots->use(player_speed);
      inventory_vector.erase(inventory_vector.begin() + item_select_variable);
      cout << "Press ENTER to continue...";
      cin.get();
    }
    inventory_vector.erase(inventory_vector.begin() + item_select_variable);
  }
  else
  {
    cout << "Invalid item index option" << endl;
  }
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
  "Choose an option (ENTER 0 to exit): ";
}
void overwrite_save()
{
savefile_object.open("magian_save.txt", ios::in);
    if (savefile_object.is_open())
    {
      char choice_new_game_variable; 
      cout << "Starting new game will Erase old save file. Are you sure? type Y to erase or any other key to Exit: ";
      cin >> choice_new_game_variable;
      if (choice_new_game_variable == 'Y' || choice_new_game_variable == 'y')
      {
        savefile_object.close();
        filesystem::remove("magian_save.txt");
        startgame();
      }
      else
      {
        menu();
      }
    }
    else 
    {
      startgame();
    }  
}
void menu() 
{
  welcome();
  int menu_variable = cin_valid_input();
  switch (menu_variable) 
  {
  case 0:
    exit(1);
  case 1:
    overwrite_save();
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