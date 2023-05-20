// Windows OS specific headers below
// In input() the command _kbhit uses user input to move key
#include <conio.h>
#include <windows.h>
// POSIX (Unix) based (Linux/MacOS/etc.) OS specific headers below
#include <unistd.h>
// Any OS headers below
// for seed random srand() function
#include <ctime>
#include <iostream>
#include <string>
// for finding save game exists and delete option save_load_game()
#include <filesystem>
// settings header contains main menu settings options
#include <game_settings.h>


using namespace std;
/* Global Variables */
double version = 0.2;
bool gameover = false;
int lives = 3;
int difficulty=3;
int language=1;
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
  "MAIN MENU\n"
  "1. Start Game\n"
  "2. Settings\n"
  "Choose an option (ENTER 0 to exit): ";
}
// Function for help on how to play game
void help() 
{
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
    //startgame();
    break;
  case 2:
    change_settings(lives, language);
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