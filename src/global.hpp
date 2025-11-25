#include <iostream>
#include <string>
#include <cstdio>
#include <raylib.h>
#include <raymath.h>

#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 900

enum GameStates { Menu, Round, RoundInt, GameOver }; // Different screens of the game.
enum MenuOptions { CPUMatch, PlayerMatch, CPUVsCPU, FirstTo, FirstToSelected, SoundToggle, Exit }; // Menu States.
enum Direction { Neutral, Up, Down }; // Direction states used for the CPU player.

class Ball {
    public:
    float width = 0.035f; 
    Vector2 positionPercentage, speed;

    void Draw();
    void Update();
};
extern Ball ball;

class Paddle {
    public:
    float width = 0.03f, height = 0.2f, speed = 0.022f;
    Vector2 positionPercentage, size;
    int score = 0, cpuUpdateTimer = 0;
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
extern float scaleFocus;

// menu.cpp
extern bool audioEnabled;
void MainMenu();

// game.cpp
extern Sound paddleHit, wallHit, outOfBounds;
extern bool lastWinningSideRight;
void Reset();
void Game();

// draw.cpp
extern char firstToText[12], p1ScoreText[2], p2ScoreText[2];
void Draw();