#include "global.hpp"

bool audioEnabled = true; 

// Starts a new game.
void NewGame() {
    Reset(); 
    lastWinningSideRight = false; 
    player1.score = 0; player2.score = 0; 
    screenWaitTimer = 60; 
    isEntryScreen = true; 
    gameState = RoundInt; 
}

// The main menu.
void MainMenu() {
    switch (selectedMode) {
        case CPUMatch: 
            if (IsKeyPressed(KEY_SPACE)) { gameState = RoundInt; screenWaitTimer = 60; EndDrawing(); NewGame();}
            else if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) { selectedMode = PlayerMatch; }
            break;
        case PlayerMatch:
            if (IsKeyPressed(KEY_SPACE)) { gameState = RoundInt; screenWaitTimer = 60; EndDrawing(); NewGame(); }
            else if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) { selectedMode = CPUMatch; } else if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) { selectedMode = CPUVsCPU; }
            break;
        case CPUVsCPU:
            if (IsKeyPressed(KEY_SPACE)) { gameState = RoundInt; screenWaitTimer = 60; EndDrawing(); NewGame(); }
            else if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) { selectedMode = PlayerMatch; } else if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) { selectedMode = FirstTo; }
            break;
        case FirstTo:
            if (IsKeyPressed(KEY_SPACE)) { selectedMode = FirstToSelected; }
            else if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) { selectedMode = CPUVsCPU; } else if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) { selectedMode = SoundToggle; }
            break;
        case FirstToSelected:
            if (IsKeyPressed(KEY_SPACE)) {  selectedMode = FirstTo; }
            else if ((IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP) || IsKeyPressedRepeat(KEY_W) || IsKeyPressedRepeat(KEY_UP)) && firstTo < 99) { firstTo += 1; if (audioEnabled) { PlaySound(wallHit); } } 
            else if ((IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN) || IsKeyPressedRepeat(KEY_S) || IsKeyPressedRepeat(KEY_DOWN)) && firstTo > 0) { firstTo -= 1; if (audioEnabled) { PlaySound(wallHit); } }
            break;
        case SoundToggle:
            if (IsKeyPressed(KEY_SPACE)) { audioEnabled = !audioEnabled; }
            else if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) { selectedMode = FirstTo; } else if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) { selectedMode = Exit; }
            break;
            break;
        case Exit:
            if (IsKeyPressed(KEY_SPACE)) { gameShouldClose = true; }
            else if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) { selectedMode = SoundToggle; }
            break;
    }
    if (audioEnabled) {
        if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) { PlaySound(wallHit); }
        if (selectedMode != Exit && IsKeyPressed(KEY_SPACE)) { PlaySound(paddleHit); }
    }
}