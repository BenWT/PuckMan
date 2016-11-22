//
// Copyright (c) 2016 by Ben Townshend. All Rights Reserved.
//

#include "Player.h"
#include "Globals.h"

void Player::MoveUp() {
    if (tileExists(tile - Globals::TILE_ROWS)) {
        tile -= Globals::TILE_ROWS;
        position->y -= Globals::TILE_SIZE;
        CalculateRect();
    }
}
void Player::MoveDown() {
    if (tileExists(tile + Globals::TILE_ROWS)) {
        tile += Globals::TILE_ROWS;
        position->y += Globals::TILE_SIZE;
        CalculateRect();
    }
}
void Player::MoveLeft() {
    if (tileExists(tile - 1)) {
        tile -= 1;
        position->x -= Globals::TILE_SIZE;
        CalculateRect();
    }
}
void Player::MoveRight() {
    if (tileExists(tile + 1)) {
        tile += 1;
        position->x += Globals::TILE_SIZE;
        CalculateRect();
    }
}

bool Player::tileExists(int currentIndex) {
    if (currentIndex >= 0 && currentIndex < Globals::TILE_COUNT) {
        return true;
    }
    return false;
}
bool Player::firstInRow(int currentIndex) {
    if (currentIndex % Globals::TILE_ROWS == 0) {
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
