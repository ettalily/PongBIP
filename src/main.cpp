#include "global.hpp"

MenuOptions selectedMode = CPUMatch;
GameStates gameState = Menu;

int screenWaitTimer = 0;
bool isEntryScreen = true, gameShouldClose = false;
float scaleFocus;

// Manages the game state.
int main() {
    // Sets window properties.
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Pang"); 
    SetTargetFPS(60);

    // Initializes audio device and loads sounds.
    InitAudioDevice();
    paddleHit = LoadSound("sound/paddlehit.wav");
    wallHit = LoadSound("sound/wallhit.wav");
    outOfBounds = LoadSound("sound/outofbounds.wav");
    
    // Sets paddle identities
    player1.isPlayer1 = true;
    player2.isPlayer1 = false;
    player1.positionPercentage.x = 0.015f;
    player2.positionPercentage.x = 0.985f;

    // Runs onces per frame.
    while(!gameShouldClose && !WindowShouldClose()) {
        if (IsKeyPressed(KEY_ENTER)) { ToggleBorderlessWindowed(); }
        if (GetScreenHeight() <= GetScreenWidth()) { scaleFocus = GetScreenHeight(); } else { scaleFocus = GetScreenWidth(); }
        // Manages what part of the code should be running in different states.
        switch (gameState) {
            case Menu: MainMenu(); break;
            case Round: Game(); break;
            case RoundInt: if (screenWaitTimer > 0) { screenWaitTimer -= 1; } else { isEntryScreen = false; gameState = Round; } break;
            case GameOver: if (screenWaitTimer > 0) { screenWaitTimer -= 1; } else { sprintf(firstToText, "First to: %d", firstTo); gameState = Menu; } break;
        }
        Draw();
    }

    UnloadSound(paddleHit);
    UnloadSound(wallHit);
    UnloadSound(outOfBounds);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}