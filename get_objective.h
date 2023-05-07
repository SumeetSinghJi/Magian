#ifndef GET_OBJECTIVE_H
#define GET_OBJECTIVE_H

#include <iostream>

extern int level_select_variable;

//function definition
void check_objective()
{
  if(level_select_variable==1) 
    {
        std::cout << "Collect 10 scrolls to return back to the Magian school to study." << std::endl;
    }
    if(level_select_variable==2) 
    {
        std::cout << "Your home is on fire. You should search for survivors." << std::endl;
    }
    if(level_select_variable==3) 
    {
        std::cout << "Shikaar 'the hunt' is on for revenge, find the culprit responsible and put an end to their lives" << std::endl;
    }
    if(level_select_variable==4) 
    {
        std::cout << "You were captured and taken aboard a foreign ship. You must escape!" << std::endl;
    }
}

#endif