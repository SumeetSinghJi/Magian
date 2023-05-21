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
class enemy_class {
public:
    string enemy_name;
    string enemy_description;
    int damage;
    int health;
    int speed;
    int enemy_x_pos;
    int enemy_y_pos;
    bool alive;
    int enemy_pause;

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

    void check_collision(int player_x, int player_y, int& player_lives) {
        if (enemy_x_pos == player_x && enemy_y_pos == player_y) {
            player_lives--;
            enemy_pause = 5; // Pause for 5 ticks
        }
    }
};
enemy_class level_1_enemy_object;
enemy_class level_2_enemy_object;
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
// players position
int x_pos, y_pos;
int lives = 3;
int difficulty=3;
clock_t lastShootTime; // shoot() time management variables
const int shootInterval = 2000; // 1 second in milliseconds
int language=1;
int player_speed=1;
char buffer[height][width];
int level=11;
int level_select_variable=1;
void menu();
vector<shared_ptr<enemy_class>> enemies_vector;
void shoot(int width, int height, int x_pos, int y_pos, edirection direction);
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
  enemies_vector.clear();

  // level 1 setup enemy
  auto level_1_enemy_pointer = make_shared<enemy_class>();
  level_1_enemy_pointer->enemy_name="Fire";
  level_1_enemy_pointer->health = 1;
  level_1_enemy_pointer->enemy_description="A large moving flame 2 meters high burning everything it touches.";
  level_1_enemy_pointer->enemy_x_pos = rand() % width;
  level_1_enemy_pointer->enemy_y_pos = rand() % height;
  level_1_enemy_pointer->alive = true;
  level_1_enemy_pointer->enemy_pause = 0;
  enemies_vector.push_back(level_1_enemy_pointer);

  // level 2 setup enemy
  auto level_2_enemy_pointer = make_shared<enemy_class>();
  level_2_enemy_pointer->enemy_name="Flying Rakashaa";
  level_2_enemy_pointer->health = 3;
  level_2_enemy_pointer->enemy_description="A flying demon with powerfull magic.";
  level_2_enemy_pointer->enemy_x_pos = rand() % width;
  level_2_enemy_pointer->enemy_y_pos = rand() % height;
  level_2_enemy_pointer->alive = true;
  level_2_enemy_pointer->enemy_pause = 0;
  enemies_vector.push_back(level_2_enemy_pointer);

  //initialise buffer with default character ' ' (space) to avoid console buffer not clearing.
  for (int i = 0; i < height; i++)
    for (int j = 0; j < width; j++)
        buffer[i][j] = ' ';
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
            buffer[y][x] = 'E';
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
        case ' ':
            if (direction != STOP) 
                { // Add this condition to skip shoot command if direction is STOP
                    // Check if enough time has passed since the last shoot
                    clock_t current_time = clock();
                    double elapsed_time = double(current_time - lastShootTime) / CLOCKS_PER_SEC;
                    if (elapsed_time >= double(shootInterval) / 1000) {
                        shoot(width, height, x_pos, y_pos, direction);
                        lastShootTime = current_time; // Update the last shoot time
                    }
                }
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


    // Level 1 - win logic 
    if (score >= 1) 
    {
    level = 2;
    cout << "You win the level";
    level_select_variable=2;
    menu();
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

    enemies_vector[0]->random_slow_movement(width, height);
    enemies_vector[1]->random_fast_movement(width, height);

    // Check for enemy collision
    for (const auto& enemy : enemies_vector) {
        if (enemy->alive) {
            enemy->check_collision(x_pos, y_pos, lives);
        }
    }

}
void shoot(int width, int height, int x_pos, int y_pos, edirection direction) 
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
            if (buffer[targetY][targetX] == 'E') {
                // Check if the bullet hit an enemy
                for (const auto& enemy : enemies_vector) {
                    if (enemy->alive && targetX == enemy->enemy_x_pos && targetY == enemy->enemy_y_pos) {
                        enemy->health--;
                        // If enemy's health is 0 or less, set the enemy's alive property to false
                        if (enemy->health <= 0) {
                            enemy->alive = false;
                        }
                        break;
                    }
                }
            }
            // Check if the bullet hit a wall
            else if (buffer[targetY][targetX] == '#') {
                break;
            }
            else {
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
        }
        else {
            break;
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