#pragma once
#include <fstream>
#include <string>
#include <iostream>
// custom magian header for getting the date and time
#include "get_datetime.h"
using namespace std;
extern int level_select_variable;
extern double version;
extern fstream savefile_object;

//added function declaration for void menu(); for the save load function  
void menu();
// for debugging purposes
string find_host_os()
{   
    string host_OS_name_variable;
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
// This function should be called in win condition
void update_savefile_level()
{
    // open file to read the contents first
    savefile_object.open("magian_save.txt", ios::in);
    if (savefile_object.is_open())
    {
        string line;
        string level_select_variable_match = "Level_select_variable=";
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
                    savefile_object << "Creating new save file for Magian.exe: " << endl;
                    savefile_object << "Computers Operating system is: " << find_host_os() << endl;
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
        savefile_object << "Creating new save file for Magian.exe: " << get_datetime()  << endl;
        savefile_object << "Version: "<< version << endl;
        savefile_object << "Date: " << get_datetime()  << endl;
        savefile_object << "Level_select_variable=" << level_select_variable << endl;
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
        string Level_one_match_variable = "Level_select_variable=1";
        string Level_two_match_variable = "Level_select_variable=2";
        string Level_three_match_variable = "Level_select_variable=3";
        string Level_four_match_variable = "Level_select_variable=4";
        string Level_five_match_variable = "Level_select_variable=5";
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