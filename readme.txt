_________________________________________________________________________________________________

                                            How to play
_________________________________________________________________________________________________

Magian. A game of mages from the school of Agni in Kashmir mountain basin.
collect from scrolls, herbs, gold and items to build your player and advance through the level.

This game was written by Sumeet Singh, www.sumeet-singh.com
The build version for this software is: 0.1 (Test version) 5th of May 2023.

Rules
* This character represents you P or स - This is you, stay safe and collect objectives.
* Money/Scrolls look like this $ or ₹ - If you find one you get a point
* Enemies look like this E or क - If you touch them you lose a life
* Walls look like this # - If you touch them you lose a life
* To move use the keyboard buttons to walk 'W' = UP,'S' = Down,'A' = Left,'D' = RIGHT
* Make it to the end of the levels. Each level has their own goals for example, collect 10 scrolls
* or find the exit without getting killed.

_________________________________________________________________________________________________


                                            Rules

_________________________________________________________________________________________________


* enemy table
* Damage table
* item table
* price table
* skill table


_________________________________________________________________________________________________


                                            Contents

_________________________________________________________________________________________________

Modding and problem fixing tutorial

Modding means editing the games files to customise it however you want. That could be to fix problems 
known as bugs or to add changes to the software.

The heirachy of files included in the game are below
* magian.cpp - the main games source code
* magian.exe - main game binary executable. Open or run this to play game
* core_game_logic.cpp - The work version of the codes main logic
* test_of_core_logic.cpp - This is a copy of the core_game_logic to test different code
* readme.txt - this is this guide for how to play the game and modify it.


_________________________________________________________________________________________________

                                            Main menu
_________________________________________________________________________________________________

Main menu
* represented by switch(case) when staring game
* 1. Start game - core game logic draw()
* 2. Level select - bonus mode to choose any level to play
* 3. Help
* 4. Settings
    * 4.1. Difficulty switch = more or less lives
    * 4.2. Change console output language e.g. Japanese
* 5. Quit


_________________________________________________________________________________________________

                                            Functions
_________________________________________________________________________________________________

Main logic of game follows running c++ functions in sequence

void menu() 
1.0. startgame()
1.1. setup()
1.2. gameover = false; // reset the gameover flag to false
1.3. while (!gameover)
1.4. draw_level_1()
1.5. input()
1.6. logic()
1.7. Sleep(150)

2.0 Settings
2.1 Help

_________________________________________________________________________________________________

                                            Logic
_________________________________________________________________________________________________


Player
* represented by character 'P'
* To move use the keyboard buttons to walk 'W' = UP,'S' = Down,'A' = Left,'D' = RIGHT
* Has 3 lives, but can set less in settings(), difficulty switch
* Moves with keyboard input in logic()


Enemies
* represented by character 'E'
* are c++ classes
* have a speed they move on the x,y plane
* moves through rand() in logic()
* are spawned in draw()
    //draw enemy
        else if (x == enemy_object.first_enemy_x_pos && y == enemy_object.first_enemy_y_pos) {
            buffer[y][x] = 'E';
        }   
* in logic() dictate damage dealth as
    --lives;

Game world
* represented by draw()
* Surrounded by character '#' walls that hurt player causing lives--;
* in draw() spawns (P), money ($), enemy (E)

Money
* represented by character '$'
* score == money;
* used in win condition to finish level and game

Win logic
* collect enough money ($) to win





_________________________________________________________________________________________________

                                            Further support
_________________________________________________________________________________________________

This is an free c++ game hosted on GitHub free to download and play
 https://github.com/SumeetSinghJi/Magian

For support talk to the author: Sumeet Singh
Author email: kurta.kursi@gmail.com
Author can be reached through their personal website: www.sumeet-singh.com
