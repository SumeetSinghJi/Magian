#pragma once
using namespace std;
extern int level_select_variable;

void check_objective()
{
    if(level_select_variable==1) 
    {
        cout << "Collect 10 scrolls to return back to the Magian school to study." << endl;
    }
    else if(level_select_variable==2) 
    {
        cout << "Your home is on fire. You should search for survivors." << endl;
    }
    else if(level_select_variable==3) 
    {
        cout << "Shikaar 'the hunt' is on for revenge, find the culprit responsible and put an end to their lives" << endl;
    }
    else if(level_select_variable==4) 
    {
        cout << "You were captured and taken aboard a foreign ship. You must escape!" << endl;
    }
    else
    {
        cout << "You must keep searching" <<endl;
    }
}