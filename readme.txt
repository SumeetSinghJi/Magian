_________________________________________________________________________________________________

                                        
                                        MAGIAN

_________________________________________________________________________________________________


Magian. A game of mages from the school of Agni in Kashmir mountain basin.
collect from scrolls, herbs, gold and items to build your player and advance through the level.

This is an free c++ game hosted on GitHub free to download and play
https://github.com/SumeetSinghJi/Magian

For support talk to the author: Sumeet Singh
Author email: kurta.kursi@gmail.com
Author can be reached through their personal website: www.sumeet-singh.com

A stable release version 1.0 is to be hopefully released publically on GitHub by 30th June 2023.

A full graphical working copy can be expected by 2024.


_________________________________________________________________________________________________ 


                                        OBJECTIVES

_________________________________________________________________________________________________   


* This character represents you P or स - This is you, stay safe and collect objectives.
* Money/Scrolls look like this $ or ₹ - If you find one you get a point
* Enemies look like this E or क - If you touch them you lose a life
* Walls look like this # - If you touch them you lose a life

* Make it to the end of the levels. Each level has their own goals for example, collect 10 scrolls
* or find the exit without getting killed.

_________________________________________________________________________________________________ 


                                            BUTTONS

_________________________________________________________________________________________________                                            


To move use the keyboard buttons to walk 'W' = UP,'S' = Down,'A' = Left,'D' = RIGHT
Q = quit game
L = Check level objective to see goals to complete level. for example collect 10 scrolls.
I = Check inventory and use items you found.
X = Check and use skills you gained.

_________________________________________________________________________________________________


                                            CONTENT

_________________________________________________________________________________________________


ENEMIES
(LEVEL | NAME | DAMAGE | HEALTH | SPEED | DROPS)
Level 1 | 
Level 2 | 
Level 3 | 
Level 4 | 
Level 5 | 
Level 6 | 
Level 7 | 
Level 8 | 
Level 9 |
Level 10 | 
Level 11 | 

ITEMS
(LEVEL | NAME | MODIFIERS)
Level 1 | 
Level 2 | 
Level 3 | 
Level 4 | 
Level 5 | 
Level 6 | 
Level 7 | 
Level 8 | 
Level 9 |
Level 10 | 
Level 11 | 

SKILLS
(LEVEL | NAME | MODIFIERS)
Level 1 | 
Level 2 | 
Level 3 | 
Level 4 | 
Level 5 | 
Level 6 | 
Level 7 | 
Level 8 | 
Level 9 |
Level 10 | 
Level 11 | 


_________________________________________________________________________________________________


                                            FILES

_________________________________________________________________________________________________

Modding and problem fixing tutorial

Modding means editing the games files to customise it however you want. That could be to fix 
problems known as bugs or to add changes to the software.

The files included in the game are below

* .vscode (folder) - contains the configuration files for editing the repo in VScode with minggw c++
* sound (folder) - contains music
* magian.cpp - the main games source code.
* magian.exe - main game binary executable. Open or run this to play game.
* test.cpp - For testing source code before replacing.
* test.cpp - For testing.
* readme.txt - the file you are reading right now is the readme.txt file it's a guide on playing.
* magian_save.txt - the save file
* known_bugs.txt - To eventually host online
* roadmap.txt - To eventually host online


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

SKILLS
                                            COMING SOON

INVENTORY
                                            COMING SOON

Save Load
* header magian_save_game.h called in source code magian.cpp main menu case
to create save game file magian_save.txt 

Level select
* level select variable updated after every win and written to magian_save.txt file
* level select option reads level_select_variable from file above


_________________________________________________________________________________________________

                                        Graphics
_________________________________________________________________________________________________



Icon - flame.ico taken from https://icons8.com



_________________________________________________________________________________________________

                                            Music
_________________________________________________________________________________________________

All music taken and altered from Pixabay.com
we thank the website developers greatly for their product.

Song list
1. Intro - Magian Avatar
Jungle Alien Ambient Mastered by Szegvari x Amen Break by JapanYoshiTheGamer @ pixabay.com
2. Shima-uta seige
From: Okinawa folk dance x fire sfx x long wind sfx @ pixabay.com
3. Cyber Attack
From: JuliusH on pixabay.com
4. COMING SOON
5. COMING SOON
6. COMING SOON
7. COMING SOON
8. COMING SOON
9. COMING SOON
10. COMING SOON
11. COMING SOON


_________________________________________________________________________________________________


                                            Credits

_________________________________________________________________________________________________


                                Game developer 👨🏿‍💻- Sumeet Singh 
                                       Playtester 📜- You 

                                           Graphics

                            Icon - flame.ico taken from https://icons8.com
