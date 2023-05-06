#include <iostream>
#include <fstream>
using namespace std;


int level_select_variable=1;
fstream save_file_object;

string get_datetime_function() 
{
  time_t now = time(0);
    tm* ltm = localtime(&now);
    int year = 1900 + ltm->tm_year;
    int month = 1 + ltm->tm_mon;
    int day = ltm->tm_mday;
    string datetime_variable = to_string(day) + "/" + to_string(month) + "/" + to_string(year);
    return datetime_variable;
}


void create_save_file_function()
{
    save_file_object.open("magian_save.txt", ios::out); // write to file
    if (save_file_object.is_open())
    {
        save_file_object << "New entry below: " << get_datetime_function()  << endl;
        save_file_object << "Level_select_variable=" << level_select_variable << endl;
        save_file_object.close();
    }
}

void update_save_file_level()
{
    save_file_object.open("magian_save.txt", ios::app); // append to file (don't overwite)
    if (save_file_object.is_open())
    {
        save_file_object << "New entry: " << get_datetime_function()  << endl;
        save_file_object << "Level_select_variable=" << level_select_variable << endl;
        save_file_object.close();
    }
}



void readfile_function() 
{
    save_file_object.open("magian_save.txt", ios::in); //read file contents
    if(save_file_object.is_open())
    {
        string save_file_contents_string_variable;
        while(getline(save_file_object, save_file_contents_string_variable)) 
        {
            cout << save_file_contents_string_variable << endl;          
        }

    }
    save_file_object.close();
}


// new function - match regex for level_select


int main()
{
    create_save_file_function();

    level_select_variable=2;

    update_save_file_level();

    readfile_function();
    
}


