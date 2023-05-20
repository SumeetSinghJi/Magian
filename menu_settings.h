#pragma once
#include <iostream>
using namespace std;
extern int lives;
extern int language;
// functino prototpes
int cin_valid_input();
void change_language();
void menu();
// Function for changing in game settings
void change_settings() 
{
  cout << "SETTINGS\n\n"
  "DIFFICULTY\n"
  "Choose a difficulty level\n"
  "1. Easy - 9 Life\n"
  "2. Medium - 5 Life\n"
  "3. Hard - 2 Life\n"
  "4. Story Mode - 99 lives\n\n"
  "LANGUAGE\n"
  "5. Change language\n\n"
  "Choose an option (ENTER 0 to exit): ";
  int lives_variable = cin_valid_input();
  switch (lives_variable) 
  {
  case 0:
    menu();
  case 1:
    lives = 1;
    break;
  case 2:
    lives = 2;
    break;
  case 3:
    lives = 3;
    break;
  case 4:
    lives = 99;
    break;
  case 5:
    change_language();
    break;
  default:
    cout << "Incorrect choice, try again." << endl;
    change_settings();
    break;
  }
  menu();
}
void change_language() 
{
  
    cout << "choose your language \n\n"
    "1. English \n"
    "2. 日本語 \n"
    "3. हिंदुस्तानी \n" << endl;
    int language_variable = cin_valid_input();
    switch(language_variable) 
    {
        case 1:
            language=1;
            cout << "English" <<endl;
            break;
        case 2:
            language=2;
            cout << "日本語" <<endl;
            break;
        case 3:
            language=3;
            cout << "हिंदुस्तानी" <<endl;
            break;
        default:
            cout << "Incorrect option. Returning to Settings." << endl;
            return;
    }
    change_settings();
}