#pragma once
#include <save_game.h>
using namespace std;

extern int level_select_variable;
extern int lives;
extern int difficulty;
extern int score;
extern bool gameover;

//functions declared before
void setup();
void input();
void logic();
void draw_level_1();
void draw_level_2();
void draw_level_3();
void draw_level_4();
void draw_level_5();
void draw_level_6();
void draw_level_7();
void draw_level_8();
void draw_level_9();
void draw_level_10();
void draw_level_11();

// Bonus game mode - select any level unlocked from savefile.txt
void level_select()
{
  match_savefile_level();
  
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
  cout << "Enter your choice (Enter 0 to exit): ";
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