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
#include <headers/game_settings.h>
#include <headers/level_select.h>
#include <headers/get_objective.h>
using namespace std;
// CLASSES
class Player
{
  public:
    string name = "";
    int magic = 0;
    int vitality = 0;
    int literacy = 0;
    int diplomacy = 0;
    int swimming = 0;
    int herbology = 0;
    int xp = 0;
};
class enemy_class 
{
public:
    string enemy_name;
    string enemy_description;
    int damage;
    int health;
    int speed;
    int xpgain;
    int enemy_x_pos;
    int enemy_y_pos;
    bool alive;
    int enemy_pause;
    char enemy_symbol;
    // Add random movement logic as part of the enemy_class
    void random_slow_movement(int width, int height) 
    {
        if (alive && enemy_pause == 0) {
            int random_direction = rand() % 4;
            switch (random_direction) {
            case 0: // Up
                if (enemy_y_pos > 1) {
                    enemy_y_pos--;
                }
                break;
            case 1: // Down
                if (enemy_y_pos < height - 2) {
                    enemy_y_pos++;
                }
                break;
            case 2: // Left
                if (enemy_x_pos > 1) {
                    enemy_x_pos--;
                }
                break;
            case 3: // Right
                if (enemy_x_pos < width - 2) {
                    enemy_x_pos++;
                }
                break;
            }
        } else if (enemy_pause > 0) {
            enemy_pause--;
        }
    }
    void random_fast_movement(int width, int height) 
    {
        if (alive && enemy_pause == 0) 
        {
            // Generate a random number between 0 and 9
            int random_num = rand() % 10;
            // Move the enemy only if the random number is less than 2 (20% chance)
            if (random_num < 2) 
            {
                int random_direction = rand() % 4;
                switch (random_direction) 
                {
                    case 0: // Up
                        if (enemy_y_pos > 1) 
                        {
                            enemy_y_pos--;
                            enemy_y_pos--;
                        }
                        break;
                    case 1: // Down
                        if (enemy_y_pos < height - 2) 
                        {
                            enemy_y_pos++;
                            enemy_y_pos++;
                        }
                        break;
                    case 2: // Left
                        if (enemy_x_pos > 1) 
                        {
                            enemy_x_pos--;
                            enemy_x_pos--;
                        }
                        break;
                    case 3: // Right
                        if (enemy_x_pos < width - 2) 
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
    void random_slow_chasing(int x_pos, int y_pos, int width, int height) 
    {
      // Move the enemy only if the random number is less than 4 (40% chance)
      if (alive && enemy_pause == 0)
      {
        // Generate a random number between 0 and 9
        int random_num = rand() % 10;
        if (rand() % 10 < 4) 
        {
            // Move towards player
            if (enemy_x_pos < x_pos && enemy_x_pos < width - 2)
                enemy_x_pos++;
            else if (enemy_x_pos > x_pos && enemy_x_pos > 1)
                enemy_x_pos--;
            if (enemy_y_pos < y_pos && enemy_y_pos < height - 2)
                enemy_y_pos++;
            else if (enemy_y_pos > y_pos && enemy_y_pos > 1)
                enemy_y_pos--;
        }
      }
      else if (enemy_pause > 0) 
      {
        enemy_pause--;
      }
    }
    void l2random_slow_chasing(int x_pos, int y_pos, int l2width, int l2height) 
    {
      // Move the enemy only if the random number is less than 4 (40% chance)
      if (alive && enemy_pause == 0)
      {
        // Generate a random number between 0 and 9
        int random_num = rand() % 10;
        if (rand() % 10 < 4) 
        {
            // Move towards player
            if (enemy_x_pos < x_pos && enemy_x_pos < l2width - 2)
                enemy_x_pos++;
            else if (enemy_x_pos > x_pos && enemy_x_pos > 1)
                enemy_x_pos--;
            if (enemy_y_pos < y_pos && enemy_y_pos < l2height - 2)
                enemy_y_pos++;
            else if (enemy_y_pos > y_pos && enemy_y_pos > 1)
                enemy_y_pos--;
        }
      }
      else if (enemy_pause > 0) 
      {
        enemy_pause--;
      }
    }
    void check_collision(int player_x, int player_y, int& player_lives) 
    {
        if (enemy_x_pos == player_x && enemy_y_pos == player_y) {
            player_lives--;
            enemy_pause = 5; // Pause for 5 ticks
        }
    }
};
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
class Obstacle
{
  public:
    char obstacle_symbol;
    string obstacle_name = "";
    string obstacle_description = "";
    int obstacle_vitality = 0;
};
fstream savefile_object;
enum edirection 
{
  STOP = 0, UP, DOWN, LEFT, RIGHT
};
edirection direction;
// POINTERS
vector<unique_ptr<item_class>> inventory_vector;
vector<shared_ptr<enemy_class>> enemies_vector;
vector<shared_ptr<enemy_class>> l2enemies_vector;
unique_ptr<Player> player_pointer_object = make_unique<Player>();
// VARIABLES
int magic = 1;;
int vitality = 1;
int literacy = 1;
int diplomacy = 1;
int swimming = 1;
int herbology = 1;
string version = "0.2.2";
string os_variable = "";
bool music_variable = true; 
bool gameover = false;
// level 1 map
const int width = 20;
const int height = 20;
char buffer[height][width];
// level 2 map
const int l2width = 40;
const int l2height = 40;
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
// FUNCTION PROTOTYPE/DECLARATION
void check_stats(const unique_ptr<Player>& player_pointer_object);
void menu();
void l2startgame();
void item_store(vector<unique_ptr<item_class>>& inventory_vector);
void check_items();
void check_skills();
void xp();
void shoot(int width, int height, int x_pos, int y_pos, edirection direction, unique_ptr<Player> &player_pointer_object);
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
  if (music_variable == false)
  {
    PlaySoundW(NULL, NULL, 0);
  }
  else
  {
    PlaySoundW(L"sound//music//alien-jungle.wav", NULL, SND_FILENAME | SND_ASYNC);
  }
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
  enemies_vector.clear();

  // setup obstacles
  // generate a random number of obstacles

  // level 1 setup enemy
  auto level_1_enemy_pointer = make_shared<enemy_class>();
  level_1_enemy_pointer->enemy_name="Fire";
  level_1_enemy_pointer->health = 1;
  level_1_enemy_pointer->xpgain = 1;
  level_1_enemy_pointer->enemy_description="A large moving flame 2 meters high burning everything it touches.";
  level_1_enemy_pointer->enemy_x_pos = rand() % width;
  level_1_enemy_pointer->enemy_y_pos = rand() % height;
  level_1_enemy_pointer->alive = true;
  level_1_enemy_pointer->enemy_pause = 0;
  level_1_enemy_pointer->enemy_symbol = '*';
  enemies_vector.push_back(level_1_enemy_pointer);

  // level 2 setup enemy
  auto level_2_enemy_pointer = make_shared<enemy_class>();
  level_2_enemy_pointer->enemy_name="Flying Rakashaa";
  level_2_enemy_pointer->health = 3;
  level_1_enemy_pointer->xpgain = 2;
  level_2_enemy_pointer->enemy_description="A flying demon with powerfull magic.";
  level_2_enemy_pointer->enemy_x_pos = rand() % width;
  level_2_enemy_pointer->enemy_y_pos = rand() % height;
  level_2_enemy_pointer->alive = true;
  level_2_enemy_pointer->enemy_pause = 0;
  level_2_enemy_pointer->enemy_symbol = '^';
  enemies_vector.push_back(level_2_enemy_pointer);

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
  // reset the level variables
  score=0;
  gameover = false;

  // Below srand method needs to be executed at runtime hence run in a function vs global variable;
  moneyx = rand() % l2width;
  moneyy = rand() % l2height;

  // setting position of player
  direction = STOP;
  x_pos = l2width / 2;
  y_pos = l2height / 2;

  // to refresh vectors for new game and level select
  inventory_vector.clear();
  l2enemies_vector.clear();

  // level 3 setup enemy
  auto level_3_enemy_pointer = make_shared<enemy_class>();
  level_3_enemy_pointer->enemy_name="Stalking Rakashaa";
  level_3_enemy_pointer->health = 3;
  level_3_enemy_pointer->xpgain = 1;
  level_3_enemy_pointer->enemy_description="A white large furry humanoid with sharp nails, bare arms and legs despite a furry body"
  "It's legs move exceedingly fast but stride is slow giving it the impression at any moment it could outrace and catch you."
  "The uncertanty of the humanoids actions cause you deep fear.";
  level_3_enemy_pointer->enemy_x_pos = rand() % width;
  level_3_enemy_pointer->enemy_y_pos = rand() % height;
  level_3_enemy_pointer->alive = true;
  level_3_enemy_pointer->enemy_pause = 0;
  level_3_enemy_pointer->enemy_symbol = '&';
  l2enemies_vector.push_back(level_3_enemy_pointer);

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

        // Draw all enemies if any exists at this position
        for (const auto& enemy : enemies_vector) 
        {
          if (enemy->alive && x == enemy->enemy_x_pos && y == enemy->enemy_y_pos) 
          {
            buffer[y][x] = enemy->enemy_symbol;
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
      else if (x == x_pos && y == y_pos) 
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
        for (const auto& enemy : l2enemies_vector) 
        {
          if (enemy->alive && x == enemy->enemy_x_pos && y == enemy->enemy_y_pos) 
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
            check_stats(player_pointer_object);
            cout << "Press ENTER button to Return to game" << endl;
            cin.get();
            break; 
        case 'q':
            exit(0);
            break;
        case ' ':
            if (direction != STOP) // Add this condition to skip shoot command if direction is STOP
                shoot(width, height, x_pos, y_pos, direction, player_pointer_object);
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
        case 'q':
            exit(0);
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
  } else 
  {

  }
  if (new_x_pos >= 1 && new_x_pos < l2width - 1 && new_y_pos >= 1 && new_y_pos < l2height - 1) 
  {
    // If the new position is within the bounds, update player's position
    x_pos = new_x_pos;
    y_pos = new_y_pos;
  } else 
  {

  }
    

    // Check if player has run out of lives and end the game if true
    if (lives == 0) 
    {
        cout << "You died!" << endl;
        menu();
    } else 
    {

    }

    // Level 1 score - Check if player picked up money and update score and money location if true
    if (x_pos == moneyx && y_pos == moneyy) 
    {
        score++;
        money++;
        moneyx = rand() % width-1;
        moneyy = rand() % height-1;
    } else 
    {

    }

    // Level 2 score - Check if player picked up money and update score and money location if true
    if (x_pos == moneyx && y_pos == moneyy) 
    {
        score++;
        money++;
        moneyx = rand() % l2width-1;
        moneyy = rand() % l2height-1;
    } else 
    {

    }


    // Level 1 - win logic 
    if (score >= 1) 
    {
    level = 2;
    cout << "You win the level";
    level_select_variable=2;
    xp();
    update_savefile_level();
    l2startgame();
    } else 
    {

    }

    // Level 2 - win logic - after specific time searching win condition (friend? or lover) appears
    if (score >= 10) 
    {
    level = 3;
    cout << "You win the level";
    level_select_variable=3;
    update_savefile_level();
    draw_level_3();
    } else 
    {

    }

   // Level 3 - win logic - specific enemy dies
    if (score >= 20) 
    {
      level = 4;
      cout << "You win the level";
      level_select_variable=4;
      update_savefile_level();
      draw_level_4();
    } else 
    {

    }

    // If enemy is killed, experience granted to player
    // vector<shared_ptr<enemy_class>> l2enemies_vector;

    // Fire enemy will move slow
    enemies_vector[0]->random_slow_movement(width, height);
    // Flying Rakshaa enemy will move fast
    enemies_vector[1]->random_fast_movement(width, height);
    // Check for enemy collision
    for (const auto& enemy : enemies_vector) {
        if (enemy->alive) {
            enemy->check_collision(x_pos, y_pos, lives);
        }
    }
    if (level==2) {
    // Stalking Rakshaa enemy will hunt player slowly
    l2enemies_vector[0]->random_slow_chasing(x_pos, y_pos, l2width, l2height);
    } else 
    {

    }
}
void l2startgame() 
{
  l2setup();
  lives = 3;
  
  if (os_variable == "Windows")
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
void shoot(int width, int height, int x_pos, int y_pos, edirection direction, std::unique_ptr<Player>& player_pointer_object)
{
    // Calculate the target position based on the direction
    int targetX = x_pos;
    int targetY = y_pos;

    while (true) {
        if (direction == UP)
            targetY--;
        else if (direction == DOWN)
            targetY++;
        else if (direction == LEFT)
            targetX--;
        else if (direction == RIGHT)
            targetX++;

        // Check if the target position is within the bounds of the game map
        if (targetX >= 0 && targetX < width && targetY >= 0 && targetY < height) {
            // Check if there is an enemy at the target position
            bool hitEnemy = false;
            for (const auto& enemy : enemies_vector) {
                if (enemy->alive && targetX == enemy->enemy_x_pos && targetY == enemy->enemy_y_pos) {
                    hitEnemy = true;
                    enemy->health--;
                    // If enemy's health is 0 or less, set the enemy's alive property to false
                    if (enemy->health <= 0) {
                        player_pointer_object->xp += enemy->xpgain;
                        enemy->alive = false;
                    }
                    break;
                }
            }

            if (hitEnemy) {
                // Clear the bullet from the previous position
                buffer[y_pos][x_pos] = ' ';

                // Draw the bullet at the target position
                buffer[targetY][targetX] = '*';
                draw_level_1();

                // Sleep after drawing the bullet
                Sleep(50);

                // Clear the bullet from the current position
                buffer[targetY][targetX] = ' ';

                // Update the player position to the new target position
                x_pos = targetX;
                y_pos = targetY;
            } else if (buffer[targetY][targetX] == '#') {
                // Check if the bullet hit a wall
                break;
            } else {
                // Clear the bullet from the previous position
                buffer[y_pos][x_pos] = ' ';

                // Draw the bullet at the target position
                buffer[targetY][targetX] = '*';
                draw_level_1();

                // Sleep after drawing the bullet
                Sleep(50);

                // Clear the bullet from the current position
                buffer[targetY][targetX] = ' ';

                // Update the player position to the new target position
                x_pos = targetX;
                y_pos = targetY;
            }
        } else {
            break;
        }
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
void check_skills()
{
  cout << "SKILLS\n\n"
  "Change skill to\n";
}
void check_stats(const unique_ptr<Player>& player_pointer_object)
{
  cout << "PLAYER\n"
  << "NAME: " << player_pointer_object->name << endl
  << "XP: " << player_pointer_object->xp << endl
  << "PERSONALITY\n"
  << "Magic: " << player_pointer_object->magic << endl
  << "(Damage dealt modifier to spells, gained from defeating enemies)\n"
  << "Vitality: " << player_pointer_object->vitality << endl
  << "(Vitality is your life value. Increasing helps you stay alive. Gained from experience)\n"
  << "Literacy: " << player_pointer_object->literacy << endl
  << "(Higher literacy equals better spells gained from scrolls and diciphering foreign objects\n"
  << "gained from constantly reading)\n"
  << "Diplomacy: " << player_pointer_object->diplomacy << endl
  << "(High diplomacy grants better price haggling, reduces enemy aggression\n"
  << "and grants more speech options and outcomes. Gained from trainers)\n"
  << "Swimming: " << player_pointer_object->swimming << endl
  << "(High scores grant better chance to survive swimming through long water stretches without"
  << "surfacing. Gained by constantly swimming)"
  << "Herbology: " << player_pointer_object->herbology << endl
  << "(High herbology grants better chance at identfying herbs for medicine. Gained by eating new things)";
  
}
void setup_player_header()
{
  savefile_object.open("magian_save.txt", ios::app);
  if(savefile_object.is_open())
  {
    savefile_object << "STATS" << endl;
    savefile_object.close();
  }
  else
  {
    cerr << "Error: Couldn't write header txt AVATAR to savefile";
  }
}
void setup_player()
{
  player_pointer_object->magic = magic;
  player_pointer_object->vitality = vitality;
  player_pointer_object->literacy = literacy;
  player_pointer_object->diplomacy = diplomacy;
  player_pointer_object->swimming = swimming;
  player_pointer_object->herbology = herbology;

  savefile_object.open("magian_save.txt", ios::app);
    if(savefile_object.is_open())
    {
        savefile_object << "MAGIC: " << magic << endl;
        savefile_object << "VITALITY: " << vitality << endl;
        savefile_object << "LITERACY: " << literacy << endl;
        savefile_object << "DIPLOMACY: " << diplomacy << endl;
        savefile_object << "SWIMMING: " << swimming << endl;
        savefile_object << "HERBOLOGY: " << herbology << endl;
        savefile_object.close();
    }
    else
    {
        cerr << "Error: Inventory not saved to save file" << endl;
        return;
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
  "1. New Game\n"
  "2. Continue\n"
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
void choose_name()
{
  bool name_correct_variable = false;
    while (name_correct_variable == false)
    {        
        cout << "Enter your characters name: ";
        getline(cin, name);
        if (name == "q" || name == "Q")
        {
            menu();
            return;
        }
        else if (name.length() <= 30)
        {
            player_pointer_object->name = name;
            
            name_correct_variable = true;
        }
        else 
        {
            cout << "Name must be less then 30 characters long\n"
            "Enter a new name again or enter q to quit: ";
        }
    }
    cout << "Your name is: " << name << endl;
    savefile_object.open("magian_save.txt", ios::app);
    if (savefile_object.is_open())
    {
        savefile_object << "CHARACTER NAME: " << name << endl;
        savefile_object.close();
    }
    else
    {
        cerr << "Error: failed to write character name to file" << endl;
        return;
    }
}
void xp()
{
  savefile_object.open("magian_save.txt", ios::app);
  if(savefile_object.is_open())
  {
    savefile_object << "XP: " << player_pointer_object->xp<< endl;
    savefile_object.close();
  }
  else
  {
    cerr << "Error: Couldn't write xp to savefile";
  }
}
void startgame() 
{
  choose_name();
  setup_player_header();
  setup_player();
  item_store_header();
  setup();
  lives = 3;
  if (os_variable == "Windows")
  { 
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
  else
  {
    while (!gameover) 
    {
      draw_level_1();
      POSIXinput();
      logic();
      Sleep(150);
    }
    cout << "Game Over. Your final score is: " << score << endl;
    cin.get();
  }
}
void newgame()
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
  os_variable = find_host_os();
  if (music_variable == false)
  {
    PlaySoundW(NULL, NULL, 0);
  }
  else
  {
    PlaySoundW(L"sound//music//Cyber_Attack_by_JuliusH.wav", NULL, SND_FILENAME | SND_ASYNC);
  }
  welcome();
  int menu_variable = cin_valid_input();
  switch (menu_variable)
  {
  case 0:
    exit(1);
  case 1:
    newgame();
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