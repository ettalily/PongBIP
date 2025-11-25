#include <iostream>
#include <raylib.h>

#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 900

enum GameStates { Menu, Round, RoundInt, GameOver }; // Different screens of the game.
enum MenuOptions { CPUMatch, PlayerMatch, CPUVsCPU, FirstTo, FirstToSelected, SoundToggle, Exit }; // Menu States.
enum Direction { Neutral, Up, Down }; // Direction states used for the CPU player.

class Ball {
    public:
    float x, y, width = 30; int speed_x, speed_y;

    void Draw();
    void Update();
};
extern Ball ball;

class Paddle {
    public:
    float x, y, width = 20, height = 150;
    int speed = 15, score = 0, cpuUpdateTimer = 0;
    bool isPlayer1;
    Direction moveDir = Neutral;

    void Move();
    void Draw();
    void PlayerUpdate();
    void CPUUpdate();
};
extern Paddle player1, player2;

// main.cpp
extern MenuOptions selectedMode;
extern GameStates gameState;
extern int firstTo, screenWaitTimer;
extern bool isEntryScreen, gameShouldClose;

// menu.cpp
extern bool audioEnabled;
void MainMenu();

// game.cpp
extern Sound paddleHit, wallHit, outOfBounds;
extern bool lastWinningSideRight;
void Reset();
void Game();

// draw.cpp
void Draw();