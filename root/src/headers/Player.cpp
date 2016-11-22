//
// Copyright (c) 2016 by Ben Townshend. All Rights Reserved.
//

#include "Player.h"

void Player::MoveUp(GameState* state) {
    if (tileExists(tile - TILE_ROWS)) {
        tile -= TILE_ROWS;
        position->y -= TILE_SIZE;
        CalculateRect();
    }
}
void Player::MoveDown(GameState* state) {
    if (tileExists(tile + TILE_ROWS)) {
        tile += TILE_ROWS;
        position->y += TILE_SIZE;
        CalculateRect();
    }
}
void Player::MoveLeft(GameState* state) {
    if (tileExists(tile - 1)) {
        tile -= 1;
        position->x -= TILE_SIZE;
        CalculateRect();
    }
}
void Player::MoveRight(GameState* state) {
    if (tileExists(tile + 1)) {
        tile += 1;
        position->x += TILE_SIZE;
        CalculateRect();
    }
}

bool Player::tileExists(int currentIndex) {
    if (currentIndex >= 0 && currentIndex < TILE_COUNT) {
        return true;
    }
    return false;
}
bool Player::firstInRow(int currentIndex) {
    if (currentIndex % TILE_ROWS == 0) {
        return true;
    }
    return false;
}
bool Player::lastInRow(int currentIndex) {
    return firstInRow(currentIndex + 1);
}
bool Player::shouldCollide(int newIndex) {
    // TODO Implement shouldCollide function
    return false;
}
