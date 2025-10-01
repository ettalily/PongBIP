# Pong, but it's Pong
Pretty cool, huh?

## About
*"Pong, but it's Pong"* is a recreation of Pong in C++ and Raylib made as a way for me to learn how to use both. It is rather bare-bones and simple, but hopefully works well and has quite a lot of polish. 

I plan to make some additions and changes to it in future, primarily as a way to learn how to implement certain things.

## Features
- **CPU Matches** - Has a very simple but competent AI CPU opponent.
- **Player Matches** - Can be played with another human opponent.
- **First to X Format Setting** - Can be set to a first to X format with a selected number of rounds needed to win.
- **CPU Vs CPU Matches** - Make the CPUs play against one another.

## Controls
Use the **W and S keys** to control player 1, and the **Up and Down keys** to control player 2. In singleplayer, either can be used. These keys are also used to navigate the menu.

Press the **Space Bar** to select in the menu, and the **Backspace key** during a game to end the round and exit back to the main menu.

## Building on Linux
Requires ``gcc`` for linux builds and ``mingw-w64-gcc`` for windows builds.

- To build for linux platforms, run ``bash build.bash`` in the root of the repository. 
- To build for windows platforms, run ``bash buildWin.bash`` in the root of the repository.

## Notes
- Raylib 5.5 builds for Linux and Windows for x86_64 platforms are included in the /raylib directory along with their licenses. The raylib project can be found [here](https://github.com/raysan5/raylib). I am not involved with this project.