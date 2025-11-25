#include "global.hpp"

void MenuDraw() {
    // Menu elements.
    DrawText(TextFormat("Pong, but it's Pong"), 325, (GetScreenHeight()/2) - 200, 100, WHITE);
    DrawText(TextFormat("CPU Match"), 600, (GetScreenHeight()/2) - 25, 50, WHITE);
    DrawText(TextFormat("Player Match"), 700, (GetScreenHeight()/2) + 40, 50, WHITE);
    DrawText(TextFormat("CPU vs CPU"), 600, (GetScreenHeight()/2) + 105, 50, WHITE);
    if (firstTo > 0) { DrawText(TextFormat("First to: %i", firstTo), 700, (GetScreenHeight()/2) + 170, 50, WHITE); } else { DrawText(TextFormat("First to: None"), 700, (GetScreenHeight()/2) + 170, 50, WHITE); }
    if (audioEnabled) { DrawText(TextFormat("Sound: On"), 600, (GetScreenHeight()/2) + 235, 50, WHITE); } else { DrawText(TextFormat("Sound: Off"), 600, (GetScreenHeight()/2) + 235, 50, WHITE); }
    DrawText(TextFormat("Exit the Game"), 700, (GetScreenHeight()/2) + 300, 50, WHITE);
    DrawText(TextFormat("github.com/ettalily"), 20, GetScreenHeight() - 40, 20, WHITE);

    // Menu cursor.
    switch (selectedMode) {
        case CPUMatch: DrawRectangle(500, (GetScreenHeight()/2) - 10, 20, 20, WHITE); break;
        case PlayerMatch: DrawRectangle(500, (GetScreenHeight()/2) + 55, 20, 20, WHITE); break;
        case CPUVsCPU: DrawRectangle(500, (GetScreenHeight()/2) + 120, 20, 20, WHITE); break;
        case FirstTo: DrawRectangle(500, (GetScreenHeight()/2) + 185, 20, 20, WHITE); break;
        case FirstToSelected: DrawRectangle(550, (GetScreenHeight()/2) + 185, 20, 20, GREEN); break;
        case SoundToggle: DrawRectangle(500, (GetScreenHeight()/2) + 250, 20, 20, WHITE); break;
        case Exit: DrawRectangle(500, (GetScreenHeight()/2) + 315, 20, 20, WHITE); break;
    }
}

void RoundDraw() {
    DrawText(TextFormat("%i", player1.score), (GetScreenWidth()/4) - 20, 10, 100, WHITE);
    DrawText(TextFormat("%i", player2.score), (GetScreenWidth() - (GetScreenWidth()/4)) - 20, 10, 100, WHITE);
    // Draws the first to text and shortens the line to accommodate it if a first to value is set.
    if (firstTo == 0) { DrawLine(GetScreenWidth()/2, 0, GetScreenWidth()/2, GetScreenHeight(), WHITE); } else { DrawText(TextFormat("FT%i", firstTo), (GetScreenWidth()/2) - 45, 10, 40, WHITE); DrawLine(GetScreenWidth()/2, 60, GetScreenWidth()/2, GetScreenHeight(), WHITE); }
    ball.Draw();
    player1.Draw();
    player2.Draw();
}

void RoundIntDraw() {
    if (!isEntryScreen) { if (lastWinningSideRight) { DrawRectangle(0, 0, GetScreenWidth()/2, GetScreenHeight(), GREEN); } else { DrawRectangle(GetScreenWidth()/2, 0, GetScreenWidth()/2, GetScreenHeight(), GREEN); } }
    DrawText(TextFormat("%i", player1.score), (GetScreenWidth()/4) - 30, (GetScreenHeight()/2) - 100, 200, WHITE);
    DrawText(TextFormat("%i", player2.score), (GetScreenWidth() - (GetScreenWidth()/4)) - 30, (GetScreenHeight()/2) - 100, 200, WHITE);
    DrawLine(GetScreenWidth()/2, 0, GetScreenWidth()/2, GetScreenHeight(), WHITE); 
    if (firstTo != 0) { DrawText(TextFormat("First To %i", firstTo), 30, 20, 100, WHITE); }
}

void GameOverDraw() {
    DrawText(TextFormat("%i", player1.score), (GetScreenWidth()/4) - 30, (GetScreenHeight()/2) - 100, 200, WHITE);
    DrawText(TextFormat("%i", player2.score), (GetScreenWidth() - (GetScreenWidth()/4)) - 30, (GetScreenHeight()/2) - 100, 200, WHITE);
    DrawLine(GetScreenWidth()/2, 0, GetScreenWidth()/2, GetScreenHeight(), WHITE); 

    // Displays which player wins and does so differently depending on the game mode.
    if (player1.score > player2.score) { 
        switch (selectedMode) {
            case CPUMatch: DrawText(TextFormat("You Win"), 220, 200, 100, WHITE); break;
            case PlayerMatch: DrawText(TextFormat("Player 1 Wins"), 60, 200, 100, WHITE); break;
            case CPUVsCPU: DrawText(TextFormat("CPU 1 Wins"), 120, 200, 100, WHITE); break;
        }
    } else if (player2.score > player1.score) {
        switch (selectedMode) {
            case CPUMatch: DrawText(TextFormat("CPU Wins"), (GetScreenWidth()/2) + 170, 200, 100, WHITE); break;
            case PlayerMatch: DrawText(TextFormat("Player 2 Wins"), (GetScreenWidth()/2) + 60, 200, 100, WHITE); break;
            case CPUVsCPU: DrawText(TextFormat("CPU 2 Wins"), (GetScreenWidth()/2) + 120, 200, 100, WHITE); break;
        }
    } else { DrawText(TextFormat("Game Draw"), 30, 20, 100, WHITE); }
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