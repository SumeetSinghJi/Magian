_________________________________________________________________________________________________

                                    
             ▄▄▄▄███▄▄▄▄      ▄████████    ▄██████▄   ▄█     ▄████████ ███▄▄▄▄ 
           ▄██▀▀▀███▀▀▀██▄   ███    ███   ███    ███ ███    ███    ███ ███▀▀▀██▄
           ███   ███   ███   ███    ███   ███    █▀  ███▌   ███    ███ ███   ███
           ███   ███   ███   ███    ███  ▄███        ███▌   ███    ███ ███   ███
           ███   ███   ███ ▀███████████ ▀▀███ ████▄  ███▌ ▀███████████ ███   ███
           ███   ███   ███   ███    ███   ███    ███ ███    ███    ███ ███   ███
           ███   ███   ███   ███    ███   ███    ███ ███    ███    ███ ███   ███
             █   ███   █▀    ███    █▀    ████████▀  █▀     ███    █▀   ▀█   █▀ 

_________________________________________________________________________________________________


DISCLAIMER: This game and the symbols, language, and references used represent the culture of
the Indo-Iranian people known collectivly as Aryans. We do not condone any violence or hate.


Magian. A game of mages from the fictional school of Agni in Arianna. (known as
present day Afghanistan).

Collect scrolls, herbs, skills and items to build your player and advance through the level.

This is an free c++ game hosted on GitHub free to download and play
https://github.com/SumeetSinghJi/Magian 

License: Free to download, modify, and share but not for commerical reasons,
unless for reviewing, education or through radical customisation where likeness not linked to 
original story, author or content such as using the source code to make an unrelated game.

Stable release version 1.0 expected before December 2024.

WE NEED YOUR HELP to build and test this game. Contact the author directly.

Author: Sumeet Singh
Email: kurta.kursi@gmail.com
website: TBD

_________________________________________________________________________________________________

                                    
                                            Requirements

_________________________________________________________________________________________________

SOFTWARE
Operating System: Tested on any modern (2023) Operating system in Windows/Unix environment


HARDWARE
CPU: TBD
GPU: TBD
RAM: TBD
Storage: TBD


_________________________________________________________________________________________________ 


                                            OBJECTIVES

_________________________________________________________________________________________________   


* This character represents you P or स - This is you, stay safe and collect objectives.
* Money/Scrolls look like this $ or ₹ - If you find one you get a point
* Enemies look like this E or क - If you touch them you lose a life
* Walls look like this # - If you touch them you lose a life

* Make it to the end of the levels. Each level has their own goals for example, collect 10 scrolls
* or find the exit without getting killed.

Difficulty can be adjusted within game settings. Story mode grants 99 lives to experience a
narrative driven game without constraints.




_________________________________________________________________________________________________ 


                                            BUTTONS

_________________________________________________________________________________________________                                            


To move use the keyboard buttons to walk 
'W' = MOVE UP,
'S' = MOVE DOWN,
'A' = MOVE LEFT,
'D' = MOVE RIGHT
'SPACE BAR' = Use Skill
'Q' = quit game
'L' = Check level objective to see goals to complete level. for example collect 10 scrolls.
'I' = Check inventory and use items you found.
'X' = Check and use skills you gained.



_________________________________________________________________________________________________


                                            FILES

_________________________________________________________________________________________________


The files included in the game are below


GAME
* magian.exe - main game binary executable. Open or run this to play game.
* magian_save.txt - the save file

TXT
* readme.txt - the file you are reading right now is the readme.txt file it's a guide on playing.
* roadmap.txt - Contains future planned changes to game
* update_log.txt - Contains update log to be eventually migrated to website

ASSETS
* sound (folder) - contains music
* images (folder) - contains images

SOURCE FILES
* magian.cpp - the main games source code.

HEADERS
under folder (headers)
* game_settings.h - In game settings
* save_game.h - logic for saving and loading game
* level_select.h - logic for bonus level select mode
* get_skills.h - logic for activating skills
* get_objective.h - logic for checking objective
* get_items.h - logic for using items
* get_datetime.h - logic for general datetime use

DEVELOPMENT
* known_bugs.txt - Contanins known problems in code and solutions
* .vscode (folder) - contains the configuration files for editing the code in VScode with 
minggw c++ compiler on Windows 11.
* staging.cpp - testing changes to main source code.
* staging.exe - for testing.
* core_game_logic_testing.cpp - For testing core game logic.
* core_game_logic_testing.exe - for testing.
* CMakeLists.txt - For building executables
* linux.toolchain.cmake - used by CMake for building linux executables



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

Code conforms to minimum version C++17

Player
* represented by character 'P'
* To move use the keyboard buttons to walk 'W' = UP,'S' = Down,'A' = Left,'D' = RIGHT
* Has 3 lives, but can set less in settings(), difficulty switch
* Moves with keyboard input in logic()

Enemies
* represented by characters such as 'E'
* enemies_vector is a vector of pointer enemy objects initialised through pushback of
pointer enemy objects
in setup()
* Are pointer objects in an enemies vector
* have a speed they move on the x,y plane
* are spawned in draw()
* logic() dictates speed, damage

Game world
* represented by draw()
* Surrounded by character '#' walls that hurt player causing lives--;
* in draw() spawns (P), money ($), enemy (E)
* Every level has unique objectives required to win to progress to next level
* levels get increasingly difficult with harder enemies but better items, money and Skill
opportunities to compensate.

Money
* represented by character '$'
* score == money;
* used in win conditions or to buy more items and train skills at trainers

Win logic
* follow the objectives in your log book by pressing "l" key by default.

Save Load
* header save_game.h called in source code magian.cpp main menu case
to create save game file magian_save.txt 

Level select
* level select variable updated after every win and written to magian_save.txt file
* level select option reads level_select_variable from file above

Inventory
* Inventory is a vector of pointer item objects initialised through pushback of item subclasses
in setup()
* item classes have subclasses of items e.g. potion_subclass
* check_inventory keyboard input "i" gives option to loop through vector of inventory 
and use subclass function use to use that subclasses effect e.g. potion_subclass increases lives++
* inventory vector index item is then removed through vector.erase()

Skills
* Gained throughout the game
* Activated with SPACE_BAR
* Have various effects, damage enemies, heal etc.,

_________________________________________________________________________________________________

                                            Items
_________________________________________________________________________________________________

Item name | Description | effect

* Healing Potion - restores 1 life, life + 1
* Leather boots - increases players speed by 1, speed + 1

_________________________________________________________________________________________________

                                            Skills
_________________________________________________________________________________________________


Level 1 skill - Activated with space bar, creaes a weak fireball that shoots into a single
direction burning the first thing it hits.
Effect: 1 life damage on contact
Range: single line in any direction
Cooldown: 1 second




_________________________________________________________________________________________________


                                            Enemies

_________________________________________________________________________________________________


ENEMIES
(LEVEL | NAME | DAMAGE | ATTACK TYPE | HEALTH | SPEED | DROPS | SKILLS)
Level 1 | Fire | 1 | Touch | 1 | 1 | none | none
Level 2 | flying Rakashaa | 1 | Touch | 3 | 1 | nothing | teleports when hit
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

                                            
                                            Music

_________________________________________________________________________________________________

All music taken and altered from Pixabay.com
we thank the website developers greatly for their product.

Song list
1. Intro - Magian Avatar
Jungle Alien Ambient Mastered by Szegvari x Amen Break by JapanYoshiTheGamer █ pixabay.com
2. Shima-uta seige
From: Okinawa folk dance x fire sfx x long wind sfx █ pixabay.com
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

                                            
                                            Licensing

_________________________________________________________________________________________________

Source code - Free to download, modify, and share but not for commerical reasons,
unless for reviewing, education or through radical customisation where likeness not linked to 
original story, author or content such as using the source code to make an unrelated game.

Sound - Pixabay says free to use aslong as creativly modified, you can credit artists name e.g.
author remix.

icon - Flame icon is restriced without crediting website that designed read more

Ncurses - is open source and can be used in commercial applications. It's licensed under the
MIT license, which is permissive and allows for commercial use, modification, distribution,
and private use. However, it still requires you to include the original copyright notice and
disclaimer in your distribution.

SDL 2 - TBD

OpenGL - TBD




_________________________________________________________________________________________________


                                            Credits

_________________________________________________________________________________________________

This game is given to the thankless Magi (Zoroastrian scholars) and Rishis (Vedic scholars) that
kept the knowledge of science, and culture alive in some of the most harshest environments. 

                                Game developer 👨🏿‍💻- Sumeet Singh 
                                       Playtester 📜- You 

                                           Graphics

                            Icon - flame.ico taken from https://icons8.com
            Ascii art - https://manytools.org/hacker-tools/convert-images-to-ascii-art/go/
            



                             █████████     ███████████████████████ 
                             █████████     ███████████████████████ 
                             █████████     ███████████████████████ 
                             █████████     ███████████████████████               
                             █████████     █████████               
                             █████████     █████████               
                             █████████████████████████████████████ 
                             █████████████████████████████████████ 
                             █████████████████████████████████████ 
                                           █████████     █████████ 
                                           █████████     █████████ 
                             ███████████████████████     █████████ 
                             ███████████████████████     █████████ 
                             ███████████████████████     █████████ 
                             ███████████████████████     █████████



