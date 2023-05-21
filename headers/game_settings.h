#pragma once
#include <iostream>
using namespace std;
extern int language;
// functino prototpes
int cin_valid_input();
void menu();
void change_language(int& language);

// Function for changing in game settings
void change_settings(int& lives, int& language) 
{
  cout << "SETTINGS\n\n"
  "DIFFICULTY\n"
  "Choose a difficulty level\n"
  "1. Story Mode - 99 lives\n"
  "2. Easy - 9 Life\n"
  "3. Medium - 5 Life\n"
  "4. Hard - 1 Life\n\n"
  "LANGUAGE\n"
  "5. Change language\n\n"
  "Choose an option (ENTER 0 to exit): ";
  int lives_variable = cin_valid_input();
  switch (lives_variable) 
  {
  case 0:
    menu();
  case 1:
    lives = 99;
    menu();
    break;
  case 2:
    lives = 9;
    menu();
    break;
  case 3:
    lives = 5;
    menu();
    break;
  case 4:
    lives = 1;
    menu();
    break;
  case 5:
    change_language(language);
    menu();
    break;
  default:
    cout << "Incorrect choice, try again." << endl;
    change_settings(lives, language);
    break;
  }
}

void change_language(int& language) 
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
            menu();
            break;
        case 2:
            language=2;
            cout << "日本語" <<endl;
            menu();
            break;
        case 3:
            language=3;
            cout << "हिंदुस्तानी" <<endl;
            menu();
            break;
        default:
            cout << "Incorrect option. Returning to Settings." << endl;
            menu();
            return;
    }
}