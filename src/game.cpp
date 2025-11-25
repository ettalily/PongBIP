#include "global.hpp"

Sound paddleHit, wallHit, outOfBounds;
Ball ball;
Paddle player1, player2;
bool lastWinningSideRight = true;
int firstTo = 0;

// Controls the ball's movement.
void Ball::Update() {
    positionPercentage += speed;
    // Forces the ball to bounce if it collides with the top or bottom screen edge.
    if ((speed.y < 0 && positionPercentage.y <= 0.015f) || (speed.y > 0 && positionPercentage.y >= 0.985f)) { speed.y *= -1; if (audioEnabled) { PlaySound(wallHit); } }
}

// Player input code.
void Paddle::PlayerUpdate() {
    // Singleplayer modes.
    if (isPlayer1 && selectedMode == CPUMatch) { 
        if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) { moveDir = Up; }
        else if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) { moveDir = Down; }
        else { moveDir = Neutral; }
    // Multiplayer mode.
    } else if (isPlayer1) {
        // Player 1
        if (IsKeyDown(KEY_W)) { moveDir = Up; }
        else if (IsKeyDown(KEY_S)) { moveDir = Down; }
        else { moveDir = Neutral; } 
    } else {
        // Player 2
        if (IsKeyDown(KEY_UP)) { moveDir = Up; }
        else if (IsKeyDown(KEY_DOWN)) { moveDir = Down; }
        else { moveDir = Neutral; } 
    }
    Move();
}

// CPU Logic.
void Paddle::CPUUpdate() {
    if (cpuUpdateTimer > 0) { cpuUpdateTimer -= 1; } else {
        // Decides what direction to go in based on whether the ball is close to the paddle. This only updates if it is done with it's existing commitment.
        if (positionPercentage.y - (height * 0.25f) > ball.positionPercentage.y) { moveDir = Up; } else if (positionPercentage.y + (height * 0.25f) < ball.positionPercentage.y) {moveDir = Down;} else { moveDir = Neutral; }
        // Decides how long it commits to that movement, with the CPU making smaller commitments the closer the ball is horizontally.
        cpuUpdateTimer = GetRandomValue(4, abs((positionPercentage.x * 1000) - (ball.positionPercentage.x * 150))/50);
    }
    Move();
}

// Moves the paddle in the respective direction without allowing it to move beyond the boundaries of the screen.
void Paddle::Move() {
    if (moveDir == Up) { positionPercentage.y -= speed; } if (positionPercentage.y - (height * 0.5f) < 0.0f) { positionPercentage.y = height * 0.5f; }
    if (moveDir == Down) { positionPercentage.y += speed; } if (positionPercentage.y + (height * 0.5f) > 1.0f) { positionPercentage.y = 1.0f - (height * 0.5f); }
}

// Resets scores and sets how long the game end screen stays up.
void EndGame() { screenWaitTimer = 120; gameState = GameOver; }

// Sets/Resets the ball and paddle's starting positions and values.
void Reset() {
    player1.positionPercentage.y = 0.5f;
    player2.positionPercentage.y = 0.5f;
    ball.positionPercentage = (Vector2){ 0.5f, 0.5f };

    // Update score text.
    sprintf(p1ScoreText, "%d", player1.score);
    sprintf(p2ScoreText, "%d", player2.score);

    // Sets the ball's speed values. Serves the ball towards the winner of the last round. Has a lower max speed so you don't get caught by surprise by a super fast serve.
    if (lastWinningSideRight) { ball.speed.x = GetRandomValue(-8, -12) * 0.001f; } else { ball.speed.x = GetRandomValue(8, 12) * 0.001f; }
    ball.speed.y = GetRandomValue(-15, 15) * 0.001f;
}

// Ends the round increments the score and starts the next round. If a player has passed the FT value, then calls EndGame().
void EndRound(bool rightWin) {
    lastWinningSideRight = rightWin;
    // Updates the score and checks to see if a player has won the game.
    if (lastWinningSideRight) { player1.score += 1; } else { player2.score += 1; } if (firstTo > 0) { if (player1.score == firstTo || player2.score == firstTo) { EndGame(); return; } }
    screenWaitTimer = 60; gameState = RoundInt; Reset();
}

// The main game logic.
void Game() {
    // Exits the game if backspace is pressed.
    if (IsKeyPressed(KEY_BACKSPACE)) {EndGame(); return;}

    ball.Update();
    // Calls different code for a human player and for a cpu. The cpu only moves when the ball is heading towards it.
    if (selectedMode == PlayerMatch || selectedMode == CPUMatch) { player1.PlayerUpdate(); } else if (ball.speed.x < 0) { player1.CPUUpdate(); }
    if (selectedMode == PlayerMatch) { player2.PlayerUpdate(); } else if (ball.speed.x > 0) { player2.CPUUpdate(); }
    
    // Checks collisions between the ball and the paddle and sets the new value.
    if (ball.speed.x < 0 && CheckCollisionRecs(Rectangle{(GetScreenWidth() * ball.positionPercentage.x) - (ball.width * GetScreenHeight() * 0.5f), (GetScreenHeight() * ball.positionPercentage.y) - (ball.width * GetScreenHeight() * 0.5f), GetScreenHeight() * ball.width, GetScreenHeight() * ball.width}, Rectangle{(GetScreenWidth() * player1.positionPercentage.x) - (player1.width * GetScreenHeight() * 0.5f), (GetScreenHeight() * player1.positionPercentage.y) - (player1.height * GetScreenHeight() * 0.5f), GetScreenHeight() * player1.width, GetScreenHeight() * player1.height})) { 
        ball.speed.x = GetRandomValue(8, 20) * 0.001f; ball.speed.y = GetRandomValue(-25, 25) * 0.001f;
        if (ball.positionPercentage.x < player1.positionPercentage.x) { ball.positionPercentage.x = player1.positionPercentage.x; }
        if (audioEnabled) { PlaySound(paddleHit); }
    }
    if (ball.speed.x > 0 && CheckCollisionRecs(Rectangle{(GetScreenWidth() * ball.positionPercentage.x) - (ball.width * GetScreenHeight() * 0.5f), (GetScreenHeight() * ball.positionPercentage.y) - (ball.width * GetScreenHeight() * 0.5f), GetScreenHeight() * ball.width, GetScreenHeight() * ball.width}, Rectangle{(GetScreenWidth() * player2.positionPercentage.x) - (player2.width * GetScreenHeight() * 0.5f), (GetScreenHeight() * player2.positionPercentage.y) - (player2.height * GetScreenHeight() * 0.5f), GetScreenHeight() * player2.width, GetScreenHeight() * player2.height})) {
        ball.speed.x = GetRandomValue(-8, -20) * 0.001f; ball.speed.y = GetRandomValue(-25, 25) * 0.001f;
        if (ball.positionPercentage.x < player2.positionPercentage.x) { ball.positionPercentage.x = player2.positionPercentage.x; }
        if (audioEnabled) { PlaySound(paddleHit); }
    }
    
    // Checks if the ball has passes one of the player's goal walls and draws the EndOfRound screen if it has, and draws the normal screen if it has not.
    if (ball.positionPercentage.x >= 1.00f) {
        EndRound(true);
        if (audioEnabled) { PlaySound(outOfBounds); }
        return;
    } else if ( ball.positionPercentage.x <= 0.0f ) {
        EndRound(false);
        if (audioEnabled) { PlaySound(outOfBounds); }
        return;
    }
}