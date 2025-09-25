# Pong, but it's Pong
Pretty cool, huh?

## About
*"Pong, but it's Pong"* is a recreation of Pong in C++ and Raylib made as a way for me to learn how to use both. It is rather bare-bones and simple, but hopefully works well and has quite a lot of polish. 

I plan to make some additions and changes to it in future, primarily as a way to learn how to implement certain things.

## Features
- **CPU Matches** - Has a very simple but competent AI CPU opponent.
- **Player Matches** - Can be played with another human opponent.
- **First to X Format Setting** - Can be set to a first to X format with a selected number of rounds needed to win.

### Planned Additions and Changes
- **Sound Effects and Audio** - Would help me learn how to implement and work with audio.
- **Using Multiple Scripts** - Currently all the code is in a single script as I don't know how to properly organise my code.
- **Controller Support** - Lowest on my priorities but would be useful to learn.

## Controls
Use the **W and S keys** to control player 1, and the **Up and Down keys** to control player 2. In singleplayer, either can be used. These keys are also used to navigate the menu.

Press the **Space Bar** to select in the menu, and the **Backspace key** during a game to end the round and exit back to the main menu.

## Building on Linux
Make sure you have raylib and it's dependencies installed. This can be done through a package manager e.g. ``pacman -S raylib`` or through building from source. Installation specifics can be found [here](https://github.com/raysan5/raylib/wiki/Working-on-GNU-Linux).

To build, run
``bash build.bash`` in the root of the repository.