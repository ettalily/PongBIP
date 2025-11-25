#include "global.hpp"

char firstToText[12], p1ScoreText[2], p2ScoreText[2];

// Draws text using screen percentages, so that it keeps a consistent size and position on different screen shapes and sizes.
void DrawTextScreenScaled(const char* text, float posPercentX, float posPercentY, float sizePercent, float spacingPercent, float posAlignmentPercent) {
    Vector2 textScale = MeasureTextEx(GetFontDefault(), text, GetScreenHeight() * sizePercent, GetScreenHeight() * spacingPercent);
    DrawTextEx(GetFontDefault(), text, (Vector2){ GetScreenWidth() * posPercentX, GetScreenHeight() * posPercentY } - (Vector2){ textScale.x * posAlignmentPercent, textScale.y * 0.5f }, GetScreenHeight() * sizePercent, GetScreenHeight() * spacingPercent, WHITE);
}

void MenuDraw() {
    // Menu elements.
    DrawTextScreenScaled("Pong, but it's Pong", 0.5f, 0.35f, 0.15f, 0.007f, 0.5f);
    DrawTextScreenScaled("CPU Match", 0.36f, 0.5f, 0.055f, 0.005f, 0.0f);
    DrawTextScreenScaled("Player Match", 0.64f, 0.57f, 0.055f, 0.005f, 1.0f);
    DrawTextScreenScaled("CPU vs CPU", 0.36f, 0.64f, 0.055f, 0.005f, 0.0f);
    if (firstTo > 0) { DrawTextScreenScaled(firstToText, 0.64f, 0.71f, 0.055f, 0.005f, 1.0f); } else { DrawTextScreenScaled("First to: None", 0.64f, 0.71f, 0.055f, 0.005f, 1.0f); }
    if (audioEnabled) { DrawTextScreenScaled("Sound: On", 0.36f, 0.78f, 0.055f, 0.005f, 0.0f); } else { DrawTextScreenScaled("Sound: Off", 0.36f, 0.78f, 0.055f, 0.005f, 0.0f); }
    DrawTextScreenScaled("Exit the Game", 0.64f, 0.85f, 0.055f, 0.005f, 1.0f);
    DrawTextScreenScaled("github.com/ettalily", 0.02f, 0.95f, 0.035f, 0.005f, 0.0f);

    // Menu cursor.
    switch (selectedMode) {
        case CPUMatch: DrawRectangle(GetScreenWidth() * 0.3f, (GetScreenHeight() * 0.5f) - GetScreenHeight() * 0.03f * 0.5f, GetScreenHeight() * 0.03f, GetScreenHeight() * 0.03f, WHITE); break;
        case PlayerMatch: DrawRectangle(GetScreenWidth() * 0.3f, (GetScreenHeight() * 0.57f) - GetScreenHeight() * 0.03f * 0.5f, GetScreenHeight() * 0.03f, GetScreenHeight() * 0.03f, WHITE); break;
        case CPUVsCPU: DrawRectangle(GetScreenWidth() * 0.3f, (GetScreenHeight() * 0.64f) - GetScreenHeight() * 0.03f * 0.5f, GetScreenHeight() * 0.03f, GetScreenHeight() * 0.03f, WHITE); break;
        case FirstTo: DrawRectangle(GetScreenWidth() * 0.3f, (GetScreenHeight() * 0.71f) - GetScreenHeight() * 0.03f * 0.5f, GetScreenHeight() * 0.03f, GetScreenHeight() * 0.03f, WHITE); break;
        case FirstToSelected: DrawRectangle(GetScreenWidth() * 0.35f, (GetScreenHeight() * 0.71f) - GetScreenHeight() * 0.03f * 0.5f, GetScreenHeight() * 0.03f, GetScreenHeight() * 0.03f, GREEN); break;
        case SoundToggle: DrawRectangle(GetScreenWidth() * 0.3f, (GetScreenHeight() * 0.78f) - GetScreenHeight() * 0.03f * 0.5f, GetScreenHeight() * 0.03f, GetScreenHeight() * 0.03f, WHITE); break;
        case Exit: DrawRectangle(GetScreenWidth() * 0.3f, (GetScreenHeight() * 0.85f) - GetScreenHeight() * 0.03f * 0.5f, GetScreenHeight() * 0.03f, GetScreenHeight() * 0.03f, WHITE); break;
    }
}

void RoundDraw() {
    DrawTextScreenScaled(p1ScoreText, 0.25f, 0.07f, 0.12f, 0.005f, 0.5f);
    DrawTextScreenScaled(p2ScoreText, 0.75f, 0.07f, 0.12f, 0.005f, 0.5f);
    // Draws the first to text and shortens the line to accommodate it if a first to value is set.
    if (firstTo == 0) { DrawLine(GetScreenWidth() * 0.5f, 0, GetScreenWidth() * 0.5f, GetScreenHeight(), WHITE); } else { DrawTextScreenScaled(firstToText, 0.5f, 0.036f, 0.052f, 0.005f, 0.5f); DrawLine(GetScreenWidth() * 0.5f, GetScreenHeight() * 0.069f, GetScreenWidth() * 0.5f, GetScreenHeight(), WHITE); }
    ball.Draw();
    player1.Draw();
    player2.Draw();
}

void RoundIntDraw() {
    if (!isEntryScreen) { if (lastWinningSideRight) { DrawRectangle(0, 0, GetScreenWidth() * 0.5f, GetScreenHeight(), GREEN); } else { DrawRectangle(GetScreenWidth() * 0.5f, 0, GetScreenWidth() * 0.5f, GetScreenHeight(), GREEN); } }
    DrawTextScreenScaled(p1ScoreText, 0.25f, 0.5f, 0.3f, 0.005f, 0.5f);
    DrawTextScreenScaled(p2ScoreText, 0.75f, 0.5f, 0.3f, 0.005f, 0.5f);
    DrawLine(GetScreenWidth() * 0.5f, 0, GetScreenWidth() * 0.5f, GetScreenHeight(), WHITE);
    if (firstTo != 0) { DrawTextScreenScaled(firstToText, 0.03f, 0.1f, 0.16f, 0.005f, 0.0f); }
}

void GameOverDraw() {
    DrawTextScreenScaled(p1ScoreText, 0.25f, 0.5f, 0.3f, 0.005f, 0.5f);
    DrawTextScreenScaled(p2ScoreText, 0.75f, 0.5f, 0.3f, 0.005f, 0.5f);
    // Displays which player wins and does so differently depending on the game mode.
    if (player1.score > player2.score) { 
        DrawLine(GetScreenWidth()/2, 0, GetScreenWidth()/2, GetScreenHeight(), WHITE); 
        switch (selectedMode) {
            case CPUMatch: DrawTextScreenScaled("You Win", 0.25f, 0.25f, 0.12f, 0.007f, 0.5f); break;
            case PlayerMatch: DrawTextScreenScaled("Player 1 Wins", 0.25f, 0.25f, 0.12f, 0.007f, 0.5f); break;
            case CPUVsCPU: DrawTextScreenScaled("CPU 1 Wins", 0.25f, 0.25f, 0.12f, 0.007f, 0.5f); break;
        }
    } else if (player2.score > player1.score) {
        DrawLine(GetScreenWidth()/2, 0, GetScreenWidth()/2, GetScreenHeight(), WHITE); 
        switch (selectedMode) {
            case CPUMatch: DrawTextScreenScaled("CPU Wins", 0.75f, 0.25f, 0.12f, 0.007f, 0.5f); break;
            case PlayerMatch: DrawTextScreenScaled("Player 2 Wins", 0.75f, 0.25f, 0.12f, 0.007f, 0.5f); break;
            case CPUVsCPU: DrawTextScreenScaled("CPU 2 Wins", 0.75f, 0.25f, 0.12f, 0.007f, 0.5f); break;
        }
    } else { DrawTextScreenScaled("Game Draw", 0.5f, 0.25f, 0.12f, 0.005f, 0.5f); }
}

void Draw() {
    BeginDrawing();
    ClearBackground(BLACK);
    switch (gameState) {
        case Round: RoundDraw(); break;
        case Menu: MenuDraw(); break;
        case RoundInt: RoundIntDraw(); break;
        case GameOver: GameOverDraw(); break;
    }
    EndDrawing();
}

void Ball::Draw() {
    DrawRectangle(x, y, width, width, WHITE);
}

void Paddle::Draw() {
    DrawRectangle(x, y, width, height, WHITE);
}