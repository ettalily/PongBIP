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
- **Dynamic Resolution Support** - Including all sorts of display shapes, sizes, and resolutions, alongside arbitrary window resize support.

## Controls
Use the **W and S keys** to control player 1, and the **Up and Down keys** to control player 2. In singleplayer, either can be used. These keys are also used to navigate the menu.

Press the **Space Bar** to select in the menu, and the **Backspace key** during a game to end the round and exit back to the main menu.

Press the **Enter** key to toggle borderless windowed mode.

## Building on Linux
Requires ``gcc`` for linux builds and ``mingw-w64-gcc`` for windows builds.

To build for linux, navigate to the root directory of the repository and run:
```
bash build.bash
```
To build for windows, navigate to the root directory of the repository and run:
```
bash buildWin.bash
```

## Building on Windows
Requires a C++ compiler, such as the ``mingw-w64-gcc`` compiler included in [w64devkit](https://github.com/skeeto/w64devkit).

To build for windows, open ``w64devkit.exe``, then navigate to the root directory of this repository and run:
```
gcc -o pongbip.exe ./src/global.hpp ./src/*.cpp -I ./raylib/include ./raylib/windows/lib/libraylib.a -lgdi32 -lwinmm
```

## Notes
- Raylib 5.5 builds for Linux and Windows for x86_64 platforms are included in the ``./raylib`` directory along with their licenses. The raylib project can be found [here](https://github.com/raysan5/raylib). I am not involved with this project.