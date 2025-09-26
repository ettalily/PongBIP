#include <iostream>
#include <raylib.h>

using namespace std;

enum GameStates { Menu, Round, RoundInt, GameOver }; // Different screens of the game.
enum MenuOptions { CPUMatch, PlayerMatch, CPUVsCPU, FirstTo, FirstToSelected, Exit }; // Menu States.
enum Direction { Neutral, Up, Down }; // Direction states used for the CPU player.

const int windowWidth = 1600; const int windowHeight = 900;

MenuOptions selectedMode = CPUMatch;
GameStates gameState = Menu;
int firstTo = 0;
int screenWaitTimer = 0;
bool lastWinningSideRight = true;
bool isEntryScreen = true;
bool gameShouldClose = false;

class Ball {
    public: 
    float x, y, width = 30; int speed_x, speed_y;

    void Draw() { DrawRectangle(x, y, width, width, WHITE); } 
    
    // Controls the ball's movement.
    void Update() { 
        x += speed_x; y += speed_y; 
        // Forces the ball to bounce if it collides with the top or bottom screen edge.
        if ((speed_y > 0 && y + width >= windowHeight) || (speed_y < 0 && y <= 0)) { speed_y *= -1; }
    }
};

class Paddle {
    private: 
    int cpuUpdateTimer = 0; 

    // Moves the paddle in the respective direction without allowing it to move beyond the boundaries of the screen.
    void Move() {
        if(moveDir == Up) { y -= speed; if (y < 0) { y = 0; } } 
        if(moveDir == Down) { y += speed; if (y > windowHeight - height) { y = windowHeight - height; } }
    }

    public: 
    float x, y, width = 20, height = 150; int speed = 15, score = 0; Direction moveDir = Neutral;

    void Draw() { DrawRectangle(x, y, width, height, WHITE); }
    
    // Player input code.
    void PlayerUpdate(bool isPlayer1) {
        if (isPlayer1 && selectedMode == CPUMatch) { if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) { moveDir = Up; } else if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) { moveDir = Down; } else { moveDir = Neutral; } }
        else if (isPlayer1) { if (IsKeyDown(KEY_W)) { moveDir = Up; } else if (IsKeyDown(KEY_S)) { moveDir = Down; } else { moveDir = Neutral; } }
        else { if (IsKeyDown(KEY_UP)) { moveDir = Up; } else if (IsKeyDown(KEY_DOWN)) { moveDir = Down; } else { moveDir = Neutral; } }
        Move();
    }

    // CPU Logic.
    void CPUUpdate(int ball_y, int ball_x) {
        if (cpuUpdateTimer > 0) { cpuUpdateTimer -= 1; } else {
            // Decides what direction to go in based on whether the ball is close to the paddle. This only updates if it is done with it's existing commitment.
            if (y - (height/4) > ball_y) { moveDir = Up; } else if (y + (height/4) < ball_y) {moveDir = Down;} else { moveDir = Neutral; }
            // Decides how long it commits to that movement, with the CPU making smaller commitments the closer the ball is horizontally.
            cpuUpdateTimer = GetRandomValue(4, abs((x+(width/2)) - ball_x)/50);
        }
        Move();
    }
};

// Game Objects
Ball ball; Paddle player1; Paddle player2;

void GameOverDraw() {
    // Draws the end screen
    BeginDrawing();
        ClearBackground(BLACK);
        DrawText(TextFormat("%i", player1.score), (windowWidth/4) - 30, (windowHeight/2) - 100, 200, WHITE);
        DrawText(TextFormat("%i", player2.score), (windowWidth - (windowWidth/4)) - 30, (windowHeight/2) - 100, 200, WHITE);
        DrawLine(windowWidth/2, 0, windowWidth/2, windowHeight, WHITE); 

        // Displays which player wins and does so differently depending on the game mode.
        if (player1.score > player2.score) { 
            switch (selectedMode) {
                case CPUMatch: DrawText(TextFormat("You Win"), 220, 200, 100, WHITE); break;
                case PlayerMatch: DrawText(TextFormat("Player 1 Wins"), 60, 200, 100, WHITE); break;
                case CPUVsCPU: DrawText(TextFormat("CPU 1 Wins"), 120, 200, 100, WHITE); break;
            }
        } else if (player2.score > player1.score) {
            switch (selectedMode) {
                case CPUMatch: DrawText(TextFormat("CPU Wins"), (windowWidth/2) + 170, 200, 100, WHITE); break;
                case PlayerMatch: DrawText(TextFormat("Player 2 Wins"), (windowWidth/2) + 60, 200, 100, WHITE); break;
                case CPUVsCPU: DrawText(TextFormat("CPU 2 Wins"), (windowWidth/2) + 120, 200, 100, WHITE); break;
            }
        } else { DrawText(TextFormat("Game Draw"), 30, 20, 100, WHITE); }
    EndDrawing();
}

// Resets scores and sets how long the game end screen stays up.
void EndGame() { GameOverDraw(); screenWaitTimer = 120; gameState = GameOver; }

// Draws the round end UI.
void ScorecardDraw() {
    BeginDrawing();
        ClearBackground(BLACK);
        if (!isEntryScreen) { if (lastWinningSideRight) { DrawRectangle(0, 0, windowWidth/2, windowHeight, GREEN); } else { DrawRectangle(windowWidth/2, 0, windowWidth/2, windowHeight, GREEN); } }
        DrawText(TextFormat("%i", player1.score), (windowWidth/4) - 30, (windowHeight/2) - 100, 200, WHITE);
        DrawText(TextFormat("%i", player2.score), (windowWidth - (windowWidth/4)) - 30, (windowHeight/2) - 100, 200, WHITE);
        DrawLine(windowWidth/2, 0, windowWidth/2, windowHeight, WHITE); 
        if (firstTo != 0) { DrawText(TextFormat("First To %i", firstTo), 30, 20, 100, WHITE); }
    EndDrawing();
}

// Sets/Resets the ball and paddle's starting positions and values.
void Reset(bool endOfGame) {
    player1.y = (windowHeight/2) - (player1.height/2);
    player2.y = (windowHeight/2) - (player2.height/2);
    ball.x = windowHeight/2; ball.y = windowHeight/2;

    // Resets scores if the game is over.
    if (endOfGame) { player1.score = 0; player2.score = 0; isEntryScreen = true; lastWinningSideRight = false; }

    // Sets the ball's speed values. Serves the ball towards the winner of the last round. Has a lower max speed so you don't get caught by surprise by a super fast serve.
    if (lastWinningSideRight) { ball.speed_x = GetRandomValue(-12, -18); } else { ball.speed_x = GetRandomValue(12, 18); } ball.speed_y = GetRandomValue(-15, 15);
}

// Ends the round increments the score and starts the next round. If a player has passed the FT value, then calls EndGame().
void EndRound(bool rightWin) {
    lastWinningSideRight = rightWin;
    // Updates the score and checks to see if a player has won the game.
    if (lastWinningSideRight) { player1.score += 1; } else { player2.score += 1; } if (firstTo > 0) { if (player1.score == firstTo || player2.score == firstTo) { EndGame(); return; } }
    ScorecardDraw();
    screenWaitTimer = 60; gameState = RoundInt; Reset(false);
}

// The main game logic.
void Game() {
    // Exits the game if backspace is pressed.
    if (IsKeyPressed(KEY_BACKSPACE)) {EndGame(); return;}

    ball.Update();
    // Calls different code for a human player and for a cpu. The cpu only moves when the ball is heading towards it.
    if (selectedMode == PlayerMatch || selectedMode == CPUMatch) { player1.PlayerUpdate(true); } else if (ball.speed_x < 0) { player1.CPUUpdate(ball.y, ball.x); }
    if (selectedMode == PlayerMatch) { player2.PlayerUpdate(false); } else if (ball.speed_x > 0) { player2.CPUUpdate(ball.y, ball.x); }
    
    // Checks collisions between the ball and the paddle and sets the new value.
    if (ball.speed_x < 0 && CheckCollisionRecs(Rectangle{ball.x, ball.y, ball.width, ball.width}, Rectangle{player1.x, player1.y, player1.width, player1.height})) { ball.speed_x = GetRandomValue(12, 25); ball.speed_y = GetRandomValue(-15, 15);}
    if (ball.speed_x > 0 && CheckCollisionRecs(Rectangle{ball.x, ball.y, ball.width, ball.width}, Rectangle{player2.x, player2.y, player2.width, player2.height})) { ball.speed_x = GetRandomValue(-12, -25); ball.speed_y = GetRandomValue(-15, 15);}
    
    // Checks if the ball has passes one of the player's goal walls and draws the EndOfRound screen if it has, and draws the normal screen if it has not.
    if (ball.x >= windowWidth) { EndRound(true); return; } else if ( ball.x + ball.width <= 0 ) { EndRound(false); return; }
    else {
        // Object Drawing
        BeginDrawing();
            ClearBackground(BLACK);
            DrawText(TextFormat("%i", player1.score), (windowWidth/4) - 20, 10, 100, WHITE);
            DrawText(TextFormat("%i", player2.score), (windowWidth - (windowWidth/4)) - 20, 10, 100, WHITE);
            // Draws the first to text and shortens the line to accommodate it if a first to value is set.
            if (firstTo == 0) { DrawLine(windowWidth/2, 0, windowWidth/2, windowHeight, WHITE); } else { DrawText(TextFormat("FT%i", firstTo), (windowWidth/2) - 45, 10, 40, WHITE); DrawLine(windowWidth/2, 60, windowWidth/2, windowHeight, WHITE); }
            ball.Draw(); player1.Draw(); player2.Draw();
        EndDrawing();
    }
}

// Starts a new game.
void NewGame() { Reset(true); ScorecardDraw(); screenWaitTimer = 60; gameState = RoundInt; }

// The main menu.
void MainMenu() {
    BeginDrawing();
    ClearBackground(BLACK);
    // Manages menu navigation and draws the selection box.
    switch (selectedMode) {
        case CPUMatch: 
            if (IsKeyPressed(KEY_SPACE)) { gameState = RoundInt; screenWaitTimer = 60; EndDrawing(); NewGame(); return; }
            else if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) { selectedMode = PlayerMatch; }
            DrawRectangle(500, (windowHeight/2) - 10, 20, 20, WHITE);
            break;
        case PlayerMatch:
            if (IsKeyPressed(KEY_SPACE)) { gameState = RoundInt; screenWaitTimer = 60; EndDrawing(); NewGame(); return; }
            else if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) { selectedMode = CPUMatch; } else if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) { selectedMode = CPUVsCPU; }
            DrawRectangle(500, (windowHeight/2) + 55, 20, 20, WHITE);
            break;
        case CPUVsCPU:
            if (IsKeyPressed(KEY_SPACE)) { gameState = RoundInt; screenWaitTimer = 60; EndDrawing(); NewGame(); return; }
            else if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) { selectedMode = PlayerMatch; } else if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) { selectedMode = FirstTo; }
            DrawRectangle(500, (windowHeight/2) + 120, 20, 20, WHITE);
            break;
        case FirstTo:
            if (IsKeyPressed(KEY_SPACE)) { selectedMode = FirstToSelected; }
            else if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) { selectedMode = CPUVsCPU; } else if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) { selectedMode = Exit; }
            DrawRectangle(500, (windowHeight/2) + 185, 20, 20, WHITE);
            break;
        case FirstToSelected:
            if (IsKeyPressed(KEY_SPACE)) { selectedMode = FirstTo; }
            else if ((IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP) || IsKeyPressedRepeat(KEY_W) || IsKeyPressedRepeat(KEY_UP)) && firstTo < 99) { firstTo += 1; } 
            else if ((IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN) || IsKeyPressedRepeat(KEY_S) || IsKeyPressedRepeat(KEY_DOWN)) && firstTo > 0) { firstTo -= 1; }
            DrawRectangle(550, (windowHeight/2) + 185, 20, 20, GREEN);
            break;
        case Exit:
            if (IsKeyPressed(KEY_SPACE)) { EndDrawing(); gameShouldClose = true; return;}
            else if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) { selectedMode = FirstTo; }
            DrawRectangle(500, (windowHeight/2) + 250, 20, 20, WHITE);
            break;
    }

    // Draws the entire menu except the cursor. 
    DrawText(TextFormat("Pong, but it's Pong"), 325, (windowHeight/2) - 200, 100, WHITE);
    DrawText(TextFormat("CPU Match"), 600, (windowHeight/2) - 25, 50, WHITE);
    DrawText(TextFormat("Player Match"), 700, (windowHeight/2) + 40, 50, WHITE);
    DrawText(TextFormat("CPU vs CPU"), 600, (windowHeight/2) + 105, 50, WHITE);
    if (firstTo > 0) { DrawText(TextFormat("First to: %i", firstTo), 700, (windowHeight/2) + 170, 50, WHITE); } else { DrawText(TextFormat("First to: None"), 700, (windowHeight/2) + 170, 50, WHITE); }
    DrawText(TextFormat("Exit the Game"), 600, (windowHeight/2) + 235, 50, WHITE);
    DrawText(TextFormat("github.com/ettalily"), 20, windowHeight - 40, 20, WHITE);
    EndDrawing();
}

// Manages the game state.
int main() {
    // Sets Window Properties
    InitWindow(windowWidth, windowHeight, "Pong, but it's Pong"); SetTargetFPS(60);

    player1.x = 20; player2.x = windowWidth - (player2.width + 20);

    // Runs onces per frame.
    while(!gameShouldClose && !WindowShouldClose()) {
        // Manages what part of the code should be running in different states.
        switch (gameState) {
            case Menu: MainMenu(); break;
            case Round: Game(); break;
            case RoundInt: if (screenWaitTimer > 0) { screenWaitTimer -= 1; ScorecardDraw(); } else { isEntryScreen = false; gameState = Round; } break;
            case GameOver: if (screenWaitTimer > 0) { screenWaitTimer -= 1; GameOverDraw(); } else { gameState = Menu; } break;
        }
    }
    CloseWindow();
    return 0;
}