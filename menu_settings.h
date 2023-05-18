#pragma once
extern int lives;
extern int difficulty;
extern int language;
// calling language before settings
void change_language();
// Function for changing in game settings
void change_settings() 
{
  cout << "SETTINGS\n\n"
  "DIFFICULTY\n"
  "Choose a difficulty level\n"
  "1. Hard - 1 Life \n"
  "2. Medium - 2 Life \n"
  "3. Easy - 3 Life \n"
  //"4. Unlimited lives\n\n"
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
    lives = 3;
    break;
  case 4:
    change_language();
    break;
  case 5:
    menu();
    return;
  default:
    cout << "Incorrect choice, try again." << endl;
    change_settings();
    break;
  }
}
void change_language() 
{
  
    cout << "choose your language \n\n"
    "1. English \n"
    "2. 日本語 \n"
    "3. हिंदुस्तानी \n" << endl;
    cin >> language;
    switch(language) 
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
            cout << "Incorrect option." << endl;
            return;
    }
}
#pragma once