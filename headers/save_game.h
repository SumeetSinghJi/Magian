#pragma once
using namespace std;
// for level select function BEGIN
//functions declared before
void setup();
void input();
void logic();
void POSIXinput();
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
extern int level_select_variable;
extern int lives;
extern int difficulty;
extern int score;
extern bool gameover;
// for level select function END

extern int level_select_variable;
extern string version;
extern fstream savefile_object;
string host_OS_name_variable = "";
//added function declaration for void menu(); for the save load function  
void menu();
string get_datetime() 
{
  time_t now = time(0);
  tm* ltm = localtime(&now);
  int year = 1900 + ltm->tm_year;
  int month = 1 + ltm->tm_mon;
  int day = ltm->tm_mday;
  std::string datetime_variable = std::to_string(day) + "/" + std::to_string(month) + "/" + std::to_string(year);
  return datetime_variable;
}
// for debugging purposes
string find_host_os(string &host_OS_name_variable)
{   
    #ifdef __WIN32
        host_OS_name_variable += "Windows";
    #elif __linux__
        host_OS_name_variable += "Linux";
    #elif __APPLE__
        host_OS_name_variable += "MacOS";
    #else
        host_OS_name_variable += "Cannot detect";
    #endif
    return host_OS_name_variable; 
}
/* void write_inventory_title_in_save()
{
    //if already exists the word Inventory then Dont do below. Call in every win condition?
    savefile_object.open("magian_save.txt", ios::app);
    if (savefile_object.is_open())
    {
        savefile_object << "Inventory" << endl;
    }
    else
    {
        cerr << "Error: failed to create save file!" << endl;
        return;
    }    
} */
// This function should be called in win condition
void update_savefile_level()
{
    // open file to read the contents first
    savefile_object.open("magian_save.txt", ios::in);
    if (savefile_object.is_open())
    {
        string line;
        string level_select_variable_match = "Unlocked levels: ";
        bool match_found = false;

        while (getline(savefile_object, line))
        {
            if (line.find(level_select_variable_match) != string::npos)
            {
                // level_select_variable already exists in the save file, overwrite it
                savefile_object.close();
                savefile_object.open("magian_save.txt", ios::out);
                if (savefile_object.is_open())
                {
                    savefile_object << "Updating save file" << endl;
                    savefile_object << "Computers Operating system is: " << host_OS_name_variable << endl;
                    savefile_object << "Version: "<< version << endl;
                    savefile_object << "Date: " << get_datetime() << endl;
                    savefile_object << level_select_variable_match << level_select_variable << endl;
                    savefile_object.close();
                    match_found = true;
                    break;
                }
                else
                {
                    cerr << "Error: failed to overwrite save file!" << endl;
                    return;
                }
            }
        }
        savefile_object.close();

        if (match_found)
        {
            // level_select_variable was found and overwritten, return
            return;
        }
    }

    // level_select_variable was not found in the save file, append it
    savefile_object.open("magian_save.txt", ios::app);
    if (savefile_object.is_open())
    {
        savefile_object << "SAVE UPDATE" << endl;
        savefile_object << "Host OS is: " << host_OS_name_variable << endl;
        savefile_object << "Version: "<< version << endl;
        savefile_object << "Date: " << get_datetime() << endl;
        savefile_object << "Unlocked levels: " << level_select_variable << endl;
        savefile_object.close();
    }
    else
    {
        cerr << "Error: failed to create save file!" << endl;
        return;
    }
}
// This code is read in the bonus level select mode to see available levels
void match_savefile_level() 
{
    savefile_object.open("magian_save.txt", ios::in); //read file contents
    if(savefile_object.is_open())
    {
        string savefile_contents_string_variable;
        string Level_one_match_variable = "Unlocked levels: 1";
        string Level_two_match_variable = "Unlocked levels: 2";
        string Level_three_match_variable = "Unlocked levels: 3";
        string Level_four_match_variable = "Unlocked levels: 4";
        string Level_five_match_variable = "Unlocked levels: 5";
        bool match_found_bool = false;

        while(getline(savefile_object, savefile_contents_string_variable)) 
        {
            
            // Run the command below to test if savefile.txt contents are being read
            // cout << savefile_contents_string_variable << endl;
            if (savefile_contents_string_variable == Level_one_match_variable)
            {
                cout << "Access to levels 1" << level_select_variable << endl;
                match_found_bool = true;
                level_select_variable=1;
            }
            else if (savefile_contents_string_variable == Level_two_match_variable)
            {
                cout << "Access to levels 1, 2" << level_select_variable << endl;
                match_found_bool = true;
                level_select_variable=2;
            }
            else if (savefile_contents_string_variable == Level_three_match_variable)
            {
                cout << "Access to levels 1, 2, 3" << level_select_variable << endl;
                match_found_bool = true;
                level_select_variable=3;
            }
            else if (savefile_contents_string_variable == Level_four_match_variable)
            {
                cout << "Access to levels 1, 2, 3, 4" << level_select_variable << endl;
                match_found_bool = true;
                level_select_variable=4;
            }
            else if (savefile_contents_string_variable == Level_five_match_variable)
            {
                cout << "Access to levels 1, 2, 3, 4, 5" << level_select_variable << endl;
                match_found_bool = true;
                level_select_variable=5;
            }    
        }
    }
    savefile_object.close();
}
// Function for finding save files and deleting
void save_load_game()
{
    cout << "Checking to see if any saves exist" << endl;
    string filename = "magian_save.txt";
    if(filesystem::exists(filename))
    {
        cout << "Save file exists.\n\n" << filename << "\n\nDo you want to delete it? (y/n)";
        char response;
        cin >> response;
        if(response=='y'||response=='Y')
        {
            filesystem::remove(filename);
            cout << "Save game deleted" << endl;
            menu();
        }
        else
        {
            cout << "Save game not deleted" << endl;
            menu();
        }
    }
    else
    {
        cout << "Save game doesn't exist." << endl;
        menu();
    }
}
/* - If match from save file output to screen.

ifstream savefile_object("magian_save.txt");
  if (savefile_object.is_open())
  {
    string line;
    string name_savefile_variable_match = "CHARACTER NAME: ";
    while (getline(savefile_object, line))
    {
      size_t found = line.find(name_savefile_variable_match);
      // If found doesnt equal "no position" or "not found". (If found is true)
      if (found != string::npos)
      {
        // Match found, extract the word after "CHARACTER NAME: "
        string character_name = line.substr(found + name_savefile_variable_match.length());
        cout << "Character Name: " << character_name << endl;
        break;
      }
    }
    savefile_object.close();
  }
  else
  {
    cerr << "Error: failed to open save file and read Characters name" << endl;
    return;
  }

*/
void level_select(string host_OS_name_variable)
{
  match_savefile_level();
  cout << "Choose any level you've unlocked \n"; 
  // add a condition to show only levels that are below or equal to level_select
  for (int i = 1; i <= level_select_variable; i++) {
    switch (i) {
      case 1:
        cout << "1. Arianna Outskirts\n";
        break;
      case 2:
        cout << "2. Blazed home\n";
        break;
      case 3:
        cout << "3. Hunt\n";
        break;
      case 4:
        cout << "4. Captured aboard\n";
        break;
      case 5:
        cout << "5. Capsize\n";
        break;
      case 6:
        cout << "6. Underwater inferno\n";
        break;
      case 7:
        cout << "7. Final spell\n";
        break;
      case 8:
        cout << "8. Rakashaa planet\n";
        break;
      case 9:
        cout << "9. Cosmic escape\n";
        break;
      case 10:
        cout << "10. Nirvana\n";
        break;
      case 11:
        cout << "11. Restoration\n";
        break;
      default:
        break;
    }
  }
  // ask the user for their choice
  int level_select_choice_variable;
  cout << "Enter your choice (Enter 0 to exit): ";
  cin >> level_select_choice_variable;
  // only include continue(), l2continue() etc., for cases below
  switch (level_select_choice_variable)
  {
    case 0:
      menu();
      break;
    case 1:
      setup();
      lives = 3;
      if (host_OS_name_variable == "Windows")
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
      break;
    case 2:
      draw_level_2();
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