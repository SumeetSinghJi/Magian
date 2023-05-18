_________________________________________________________________________________________________

                                        
                                            MAGIAN

_________________________________________________________________________________________________

DISCLAIMER: This game and the symbols, language, and references used represent the culture of
the Indo-Iranian people known collectivly as Aryans. We do not condone any violence or hate.

Magian. A game of mages from the fictional school of Agni in Kashmir, Arianna. (known as
present day Afghanistan/Pakistan/North India).

Collect scrolls, herbs, skills and items to build your player and advance through the level.

This is an free c++ game hosted on GitHub free to download and play
https://github.com/SumeetSinghJi/Magian

website: www.sumeet-singh.com

Stable release version 1.0 expected before December 2024.

Code conforms to minimum version C++17

We need you to help build and test this game. Contact the author directly.

Author: Sumeet Singh
Email: kurta.kursi@gmail.com


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


                                            FILES

_________________________________________________________________________________________________

Modding and problem fixing tutorial

Modding means editing the games files to customise it however you want. That could be to fix 
problems known as bugs or to add changes to the software.

The files included in the game are below

* .vscode (folder) - contains the configuration files for editing the repo in VScode with minggw c++
* sound (folder) - contains music
* images (folder) - contains images
* magian.cpp - the main games source code.
* magian.exe - main game binary executable. Open or run this to play game.
* staging.cpp - For testing changes to source code before deploying to main source code
* staging.exe - for testing.
* test.cpp - For testing random code.
* test.exe - For testing.
* readme.txt - the file you are reading right now is the readme.txt file it's a guide on playing.
* magian_save.txt - the save file
* Troubleshooting.txt - Contanins known problems in code and solutions
* roadmap.txt - Contains future planned changes to game
* update_log.txt - Contains update log to be eventually migrated to website


_________________________________________________________________________________________________

                                            Main menu
_________________________________________________________________________________________________

Main menu

* represented by switch(case) when staring game
* 1. Start game - core game logic draw()
* 2. Save & Load - View and delete save game file
* 3. Level select - bonus mode to choose any level to play
* 4. Help
* 5. Settings
    * 4.1. Difficulty switch = more or less lives
    * 4.2. Change console output language e.g. Japanese
* 6. Soundtrack - browse in game music
* 7. Quit


_________________________________________________________________________________________________

                                            Game loop logic
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

Save Load
* header magian_save_game.h called in source code magian.cpp main menu case
to create save game file magian_save.txt 

Level select
* level select variable updated after every win and written to magian_save.txt file
* level select option reads level_select_variable from file above

Inventory
* Inventory is a vector of item classes initialised through pushback of item subclasses
* item classes have subclasses of items e.g. potion_subclass
* check_inventory keyboard input "i" gives option to loop through vector of inventory 
and use subclass function use to use that subclasses effect e.g. potion_subclass increases lives++
* inventory vector index item is then removed through vector.erase()

_________________________________________________________________________________________________

                                            Items
_________________________________________________________________________________________________

Item name | Description | effect

* Healing Potion - restores 1 life, life + 1
* Leather boots - increases players speed by 1, speed + 1

_________________________________________________________________________________________________

                                            Skills
_________________________________________________________________________________________________


                                            Coming soon


_________________________________________________________________________________________________


                                            Enemies

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
