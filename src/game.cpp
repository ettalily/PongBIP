#include "global.hpp"

Sound paddleHit, wallHit, outOfBounds;
Ball ball;
Paddle player1, player2;
bool lastWinningSideRight = true;
int firstTo = 0;

// Controls the ball's movement.
void Ball::Update() {
    x += speed_x; y += speed_y; 
    // Forces the ball to bounce if it collides with the top or bottom screen edge.
    if ((speed_y > 0 && y + width >= GetScreenHeight()) || (speed_y < 0 && y <= 0)) { speed_y *= -1; if (audioEnabled) { PlaySound(wallHit); } }
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
        if (y - (height * 0.25f) > ball.y) { moveDir = Up; } else if (y + (height * 0.25f) < ball.y) {moveDir = Down;} else { moveDir = Neutral; }
        // Decides how long it commits to that movement, with the CPU making smaller commitments the closer the ball is horizontally.
        cpuUpdateTimer = GetRandomValue(4, abs((x+(width * 0.5f)) - ball.x)/50);
    }
    Move();
}

// Moves the paddle in the respective direction without allowing it to move beyond the boundaries of the screen.
void Paddle::Move() {
    if(moveDir == Up) { y -= speed; if (y < 0) { y = 0; } } 
    if(moveDir == Down) { y += speed; if (y > GetScreenHeight() - height) { y = GetScreenHeight() - height; } }
}

// Resets scores and sets how long the game end screen stays up.
void EndGame() { screenWaitTimer = 120; gameState = GameOver; }

// Sets/Resets the ball and paddle's starting positions and values.
void Reset() {
    player1.y = (GetScreenHeight()/2) - (player1.height/2);
    player2.y = (GetScreenHeight()/2) - (player2.height/2);
    ball.x = GetScreenWidth()/2; ball.y = GetScreenHeight()/2;

    // Sets the ball's speed values. Serves the ball towards the winner of the last round. Has a lower max speed so you don't get caught by surprise by a super fast serve.
    if (lastWinningSideRight) { ball.speed_x = GetRandomValue(-10, -12); } else { ball.speed_x = GetRandomValue(10, 12); } ball.speed_y = GetRandomValue(-12, 12);
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
    if (selectedMode == PlayerMatch || selectedMode == CPUMatch) { player1.PlayerUpdate(); } else if (ball.speed_x < 0) { player1.CPUUpdate(); }
    if (selectedMode == PlayerMatch) { player2.PlayerUpdate(); } else if (ball.speed_x > 0) { player2.CPUUpdate(); }
    
    // Checks collisions between the ball and the paddle and sets the new value.
    if (ball.speed_x < 0 && CheckCollisionRecs(Rectangle{ball.x, ball.y, ball.width, ball.width}, Rectangle{player1.x, player1.y, player1.width, player1.height})) { 
        ball.speed_x = GetRandomValue(12, 25); ball.speed_y = GetRandomValue(-15, 15);
        if (audioEnabled) { PlaySound(paddleHit); }
    }
    if (ball.speed_x > 0 && CheckCollisionRecs(Rectangle{ball.x, ball.y, ball.width, ball.width}, Rectangle{player2.x, player2.y, player2.width, player2.height})) {
        ball.speed_x = GetRandomValue(-12, -25); ball.speed_y = GetRandomValue(-15, 15);
        if (audioEnabled) { PlaySound(paddleHit); }
    }
    
    // Checks if the ball has passes one of the player's goal walls and draws the EndOfRound screen if it has, and draws the normal screen if it has not.
    if (ball.x >= GetScreenWidth()) {
        EndRound(true);
        if (audioEnabled) { PlaySound(outOfBounds); }
        return;
    } else if ( ball.x + ball.width <= 0 ) {
        EndRound(false);
        if (audioEnabled) { PlaySound(outOfBounds); }
        return;
    }
}