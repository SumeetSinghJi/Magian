#ifndef MAGIAN_SAVE_GAME_H
#define MAGIAN_SAVE_GAME_H

#include <fstream>
#include <string>
#include <iostream>
// custom magian header for getting the date and time
#include "get_datetime_function_header.h"

using namespace std;

extern int level_select_variable;
extern double version;
extern fstream savefile_object;


// Creates a savefile.txt and enters data on game version
void create_savefile_function()
{
    savefile_object.open("magian_save.txt", ios::out); // write to file
    if (savefile_object.is_open())
    {
        savefile_object << "Creating new save file for Magian.exe"<< endl;
        savefile_object << "Version: "<< version << endl;
        savefile_object << "Date: " << get_datetime_function()  << endl;
        savefile_object << endl;
        savefile_object.close();
    }
}

// This function should be called in win condition
void update_savefile_level()
{
    savefile_object.open("magian_save.txt", ios::app); // append to file (don't overwite)
    if (savefile_object.is_open())
    {
        savefile_object << "New entry: " << get_datetime_function()  << endl;
        savefile_object << "Level_select_variable=" << level_select_variable << endl;
        savefile_object.close();
    }
}


// This code is read in the bonus level select mode to see available levels
void match_savefile_level_function() 
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
                cout << "Access to levels 1" << endl;
                match_found_bool = true;
                level_select_variable=1;
            }
            else if (savefile_contents_string_variable == Level_two_match_variable)
            {
                cout << "Access to levels 1, 2" << endl;
                match_found_bool = true;
                level_select_variable=2;
            }
            else if (savefile_contents_string_variable == Level_three_match_variable)
            {
                cout << "Access to levels 1, 2, 3" << endl;
                match_found_bool = true;
                level_select_variable=3;
            }
            else if (savefile_contents_string_variable == Level_four_match_variable)
            {
                cout << "Access to levels 1, 2, 3, 4" << endl;
                match_found_bool = true;
                level_select_variable=4;
            }
            else if (savefile_contents_string_variable == Level_five_match_variable)
            {
                cout << "Access to levels 1, 2, 3, 4, 5" << endl;
                match_found_bool = true;
                level_select_variable=5;
            }    
        }
    }
    savefile_object.close();
}


#endif