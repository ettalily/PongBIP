#include <iostream>
#include <raylib.h>

using namespace std;

// Different screens of the game.
enum GameStates { Menu, Round, RoundInt, GameOver };
// Menu States.
enum MenuOptions { CPUMatch, PlayerMatch, FirstTo, FirstToSelected, Exit };
// Direction states used for the CPU player.
enum Direction { Neutral, Up, Down };

GameStates gameState = Menu;
bool multiplayer;
int firstTo = 0;
int screenWaitTimer = 0;
bool lastWinningSideRight = false;
bool isEntryScreen = true;

class Ball {
    public: 
    float x, y, width = 30; int speed_x, speed_y;
    void Draw() { DrawRectangle(x, y, width, width, WHITE); } 
    
    // Controls the ball's movement.
    void Update() { 
        x += speed_x; y += speed_y; 
        // Forces the ball to bounce if it collides with the top or bottom screen edge.
        if ((speed_y > 0 && y + width >= GetScreenHeight()) || (speed_y < 0 && y <= 0)) { speed_y *= -1; }
    }
};

class PlayerOnePaddle {
    public: 
    float x, y, width = 20, height = 150; int speed = 15, score = 0;
    void Draw() { DrawRectangle(x, y, width, height, WHITE); }

    // Player one control code.
    void Update() {
        // Moves the player in the respective direction without allowing them to move beyond the boundaries of the screen.
        if(IsKeyDown(KEY_W) || (!multiplayer && IsKeyDown(KEY_UP))) { y -= speed; if (y < 0) { y = 0; } } 
        if(IsKeyDown(KEY_S) || (!multiplayer && IsKeyDown(KEY_DOWN))) { y += speed; if (y > GetScreenHeight() - height) { y = GetScreenHeight() - height; } }
    }
};

class PlayerTwoPaddle {
    private: 
    Direction decidedDirection = Neutral; int updateTimer = 0; 

    public: 
    float x, y, width = 20, height = 150; int speed = 15, score = 0;
    void Draw() { DrawRectangle(x, y, width, height, WHITE); }

    // Player two control code that only runs in multiplayer.
    void PlayerUpdate() {
        // Moves the player in the respective direction without allowing them to move beyond the boundaries of the screen.
        if(IsKeyDown(KEY_UP)) { y -= speed; if (y < 0) { y = 0; } } 
        if(IsKeyDown(KEY_DOWN)) { y += speed; if (y > GetScreenHeight() - height) { y = GetScreenHeight() - height; } }
    }

    // CPU AI code that only runs in singleplayer.
    void CPUUpdate(int ball_y, int ball_x) {
        if (updateTimer > 0) { updateTimer -= 1; } else {
            // Decides what direction to go in based on whether the ball is close to the paddle. This only updates if it is done with it's existing commitment.
            if (y - (height/4) > ball_y) { decidedDirection = Up; } else if (y + (height/4) < ball_y) {decidedDirection = Down;} else { decidedDirection = Neutral; }
            // Decides how long it commits to that movement, with the CPU making smaller commitments the closer the ball is horizontally.
            updateTimer = GetRandomValue(4, (GetScreenWidth() - ball_x)/50);
        }
        // Moves in the direction it has committed to without allowing movement beyond the boundaries of the screen.
        if (decidedDirection == Up) { y -= speed; if (y < 0) { y = 0; } } else if (decidedDirection == Down) { y += speed; if (y > GetScreenHeight() - height) { y = GetScreenHeight() - height; } }
    }
};

// Game Objects
Ball ball;
PlayerOnePaddle player1;
PlayerTwoPaddle player2;

// Sets/Resets the ball and paddle's starting positions and values.
void Reset() {
    player1.x = 20; player1.y = (GetScreenHeight()/2) - (player1.height/2);
    player2.x = (GetScreenWidth()) - (player2.width + 20); player2.y = (GetScreenHeight()/2) - (player2.height/2);
    ball.x = GetScreenWidth()/2; ball.y = GetScreenHeight()/2;

    // Sets the ball's speed values. Serves the ball towards the winner of the last round. Has a lower max speed so you don't get caught by surprise by a super fast serve.
    if (lastWinningSideRight) { ball.speed_x = GetRandomValue(-12, -18); } else { ball.speed_x = GetRandomValue(12, 18); } ball.speed_y = GetRandomValue(-15, 15);
}

void GameOverDraw() {
    // Draws the end screen
    BeginDrawing();
        ClearBackground(BLACK);
        DrawText(TextFormat("%i", player1.score), (GetScreenWidth()/4) - 30, (GetScreenHeight()/2) - 100, 200, WHITE);
        DrawText(TextFormat("%i", player2.score), (GetScreenWidth() - (GetScreenWidth()/4)) - 30, (GetScreenHeight()/2) - 100, 200, WHITE);
        DrawLine(GetScreenWidth()/2, 0, GetScreenWidth()/2, GetScreenHeight(), WHITE); 

        // Displays which player wins and does so differently depending on whether the game is multiplayer or not.
        if (player1.score > player2.score) { if (multiplayer) { DrawText(TextFormat("Player 1 Wins"), 30, 20, 100, WHITE);} else { DrawText(TextFormat("You Win"), 30, 20, 100, WHITE);} }
        else if (player2.score > player1.score) { if (multiplayer) { DrawText(TextFormat("Player 2 Wins"), 30, 20, 100, WHITE);} else { DrawText(TextFormat("CPU Wins"), 30, 20, 100, WHITE); } }
        else { DrawText(TextFormat("Game Draw"), 30, 20, 100, WHITE); }
    EndDrawing();
}

// Resets scores and sets how long the game end screen stays up.
void EndGame() { GameOverDraw(); screenWaitTimer = 120; gameState = GameOver;}

// Draws the round end UI.
void ScorecardDraw() {
    BeginDrawing();
        ClearBackground(BLACK);
        if (!isEntryScreen) { if (lastWinningSideRight) { DrawRectangle(0, 0, GetScreenWidth()/2, GetScreenHeight(), GREEN); } else { DrawRectangle(GetScreenWidth()/2, 0, GetScreenWidth()/2, GetScreenHeight(), GREEN); } }
        DrawText(TextFormat("%i", player1.score), (GetScreenWidth()/4) - 30, (GetScreenHeight()/2) - 100, 200, WHITE);
        DrawText(TextFormat("%i", player2.score), (GetScreenWidth() - (GetScreenWidth()/4)) - 30, (GetScreenHeight()/2) - 100, 200, WHITE);
        DrawLine(GetScreenWidth()/2, 0, GetScreenWidth()/2, GetScreenHeight(), WHITE); 
        if (firstTo != 0) { DrawText(TextFormat("First To %i", firstTo), 30, 20, 100, WHITE); }
    EndDrawing();
}

// Starts a new game.
void NewGame() {
    // Resets player scores and other values.
    player1.score = 0; player2.score = 0; isEntryScreen = true; lastWinningSideRight = false;
    // Draws the scorecard.
    ScorecardDraw();
    // Sets how long the score screen remains up and resets object positions.
    screenWaitTimer = 60; gameState = RoundInt; Reset();
}

// Ends the round increments the score and starts the next round. If a player has passed the FT value, then calls EndGame().
void EndRound(bool rightWin) {
    lastWinningSideRight = rightWin;
    // Updates the score and checks to see if a player has won the game.
    if (lastWinningSideRight) { player1.score += 1; } else { player2.score += 1; } if (firstTo > 0) { if (player1.score == firstTo || player2.score == firstTo) { EndGame(); return; } }
    // Draws the Scorecard
    ScorecardDraw();
    // Sets how long the score screen remains up and resets object positions.
    screenWaitTimer = 60; gameState = RoundInt; Reset();
}

// The main game logic.
void Game() {
    // Exits the game if backspace is pressed.
    if (IsKeyPressed(KEY_BACKSPACE)) {EndGame(); return;}

    ball.Update();
    player1.Update();
    // Calls different code for a human player and for a cpu. The cpu only moves when the ball is heading towards it.
    if (multiplayer) { player2.PlayerUpdate(); } else if (ball.speed_x > 0) { player2.CPUUpdate(ball.y, ball.x); }
    
    // Checks collisions between the ball and the paddle and sets the new value.
    if (ball.speed_x < 0 && CheckCollisionRecs(Rectangle{ball.x, ball.y, ball.width, ball.width}, Rectangle{player1.x, player1.y, player1.width, player1.height})) { ball.speed_x = GetRandomValue(12, 25); ball.speed_y = GetRandomValue(-15, 15);}
    if (ball.speed_x > 0 && CheckCollisionRecs(Rectangle{ball.x, ball.y, ball.width, ball.width}, Rectangle{player2.x, player2.y, player2.width, player2.height})) { ball.speed_x = GetRandomValue(-12, -25); ball.speed_y = GetRandomValue(-15, 15);}
    
    // Checks if the ball has passes one of the player's goal walls and draws the EndOfRound screen if it has, and draws the normal screen if it has not.
    if (ball.x >= GetScreenWidth()) { EndRound(true); return; } else if ( ball.x + ball.width <= 0 ) { EndRound(false); return; }
    else {
        // Object Drawing
        BeginDrawing();
            ClearBackground(BLACK);
            DrawText(TextFormat("%i", player1.score), (GetScreenWidth()/4) - 20, 10, 100, WHITE);
            DrawText(TextFormat("%i", player2.score), (GetScreenWidth() - (GetScreenWidth()/4)) - 20, 10, 100, WHITE);
            // Draws the first to text and shortens the line to accommodate it if a first to value is set.
            if (firstTo == 0) { DrawLine(GetScreenWidth()/2, 0, GetScreenWidth()/2, GetScreenHeight(), WHITE); } else { DrawText(TextFormat("FT%i", firstTo), (GetScreenWidth()/2) - 45, 10, 40, WHITE); DrawLine(GetScreenWidth()/2, 60, GetScreenWidth()/2, GetScreenHeight(), WHITE); }
            ball.Draw(); player1.Draw(); player2.Draw();
        EndDrawing();
    }
}

// The main menu.
class MainMenu {
    public:
    MenuOptions selected = CPUMatch;

    void UpdateAndDraw() {
        BeginDrawing();
        ClearBackground(BLACK);
        // Manages menu navigation and draws the selection box.
        switch (selected) {
            case CPUMatch: 
                if (IsKeyPressed(KEY_SPACE)) { multiplayer = false; gameState = RoundInt; screenWaitTimer = 60; EndDrawing(); NewGame(); return; }
                else if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) { selected = PlayerMatch; }
                DrawRectangle(500, (GetScreenHeight()/2) + 15, 20, 20, WHITE);
                break;
            case PlayerMatch:
                if (IsKeyPressed(KEY_SPACE)) { multiplayer = true; gameState = RoundInt; screenWaitTimer = 60; EndDrawing(); NewGame(); return; }
                else if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) { selected = CPUMatch; } else if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) { selected = FirstTo; }
                DrawRectangle(500, (GetScreenHeight()/2) + 90, 20, 20, WHITE);
                break;
            case FirstTo:
                if (IsKeyPressed(KEY_SPACE)) { selected = FirstToSelected; }
                else if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) { selected = PlayerMatch; } else if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) { selected = Exit; }
                DrawRectangle(500, (GetScreenHeight()/2) + 165, 20, 20, WHITE);
                break;
            case FirstToSelected:
                if (IsKeyPressed(KEY_SPACE)) { selected = FirstTo; }
                else if ((IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP) || IsKeyPressedRepeat(KEY_W) || IsKeyPressedRepeat(KEY_UP)) && firstTo < 99) { firstTo += 1; } 
                else if ((IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN) || IsKeyPressedRepeat(KEY_S) || IsKeyPressedRepeat(KEY_DOWN)) && firstTo > 0) { firstTo -= 1; }
                DrawRectangle(550, (GetScreenHeight()/2) + 165, 20, 20, GREEN);
                break;
            case Exit:
                if (IsKeyPressed(KEY_SPACE)) { EndDrawing(); CloseWindow(); }
                else if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) { selected = FirstTo; }
                DrawRectangle(500, (GetScreenHeight()/2) + 240, 20, 20, WHITE);
                break;
        }

        // Draws the entire menu except the cursor. 
        DrawText(TextFormat("Pong, but it's Pong"), 325, (GetScreenHeight()/2) - 200, 100, WHITE);
        DrawText(TextFormat("CPU Match"), 600, GetScreenHeight()/2, 50, WHITE);
        DrawText(TextFormat("Player Match"), 700, (GetScreenHeight()/2) + 75, 50, WHITE);
        if (firstTo > 0) { DrawText(TextFormat("First to: %i", firstTo), 600, (GetScreenHeight()/2) + 150, 50, WHITE); } else { DrawText(TextFormat("First to: None"), 600, (GetScreenHeight()/2) + 150, 50, WHITE); }
        DrawText(TextFormat("Exit the Game"), 700, (GetScreenHeight()/2) + 225, 50, WHITE);
        DrawText(TextFormat("github.com/ettalily"), 20, GetScreenHeight() - 40, 20, WHITE);
        EndDrawing();
    }
};

MainMenu menu;

// Manages the game state.
int main() {
    // Sets Window Properties
    const int windowWidth = 1600; const int windowHeight = 900;
    InitWindow(windowWidth, windowHeight, "Pong, but it's Pong");
    SetTargetFPS(60);

    // Runs onces per frame.
    while(!WindowShouldClose()) {
        // Manages what part of the code should be running in different states.
        switch (gameState) {
            case Menu: menu.UpdateAndDraw(); break;
            case Round: Game(); break;
            case RoundInt: if (screenWaitTimer > 0) { screenWaitTimer -= 1; ScorecardDraw(); } else { isEntryScreen = false; gameState = Round; } break;
            case GameOver: if (screenWaitTimer > 0) { screenWaitTimer -= 1; GameOverDraw(); } else { gameState = Menu; } break;
        }
    }
}