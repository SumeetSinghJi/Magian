#include <iostream>
#include <fstream>
// including magian header for saving files functions
#include <magian_save_game.h>
using namespace std;

int level_select_variable=1;
double version = 0.1;
fstream savefile_object;


int main()
{
    create_savefile_function();

    level_select_variable=2;

    update_savefile_level();

    match_savefile_level_function();

}


