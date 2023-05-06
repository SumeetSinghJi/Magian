#ifndef MAGIAN_SAVE_GAME_H
#define MAGIAN_SAVE_GAME_H

#include <fstream>
#include <string>
#include "get_datetime_function_header.h"

using namespace std;

extern int level_select_variable;
extern double version;
extern fstream savefile_object;

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

void match_savefile_level_function() 
{
    savefile_object.open("magian_save.txt", ios::in); //read file contents
    if(savefile_object.is_open())
    {
        string savefile_contents_string_variable;
        while(getline(savefile_object, savefile_contents_string_variable)) 
        {
            cout << savefile_contents_string_variable << endl;          
        }
    }

    savefile_object.close();
}


#endif