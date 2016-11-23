//
// Copyright (c) 2016 by Ben Townshend. All Rights Reserved.
//

#include <iostream>

#include "Globals.h"
#include "GameState.h"
#include "Tile.h"
#include "Player.h"

void Player::Render(SDL_Renderer* renderer) {
    SDL_Rect drawRect = destRect;
    drawRect.x += offsetX;
    drawRect.y += offsetY;
    SDL_RenderCopy(renderer, texture, (srcRect.w == -1) ? NULL : &srcRect, &drawRect);
}

void Player::DoMove(GameState& gameState, double moveAmount) {
    bool changeIndex = false;
    bool UpDown;

    if (gameState.playerMoveDirection == Up || gameState.playerMoveDirection == Down) UpDown = true;
    else UpDown = false;

    if (gameState.playerMoveDirection == Up) {
        offsetY -= moveAmount;
    } else if (gameState.playerMoveDirection == Down) {
        offsetY += moveAmount;
    } else if (gameState.playerMoveDirection == Left) {
        offsetX -= moveAmount;
    } else if (gameState.playerMoveDirection == Right) {
        offsetX += moveAmount;
    }

    if (UpDown) {
        if (offsetY > Globals::TILE_SIZE * 0.75) {
            tile += Globals::TILE_ROWS;
            offsetY -= Globals::TILE_SIZE;
            changeIndex = true;
        } else if (offsetY < -(Globals::TILE_SIZE * 0.5)) {
            tile -= Globals::TILE_ROWS;
            offsetY += Globals::TILE_SIZE;
            changeIndex = true;
        }

        if (offsetX > 2) {
            offsetY -= moveAmount;
        } else if (offsetX < -2) {
            offsetX += moveAmount;
        } else {
            offsetX = 0;
        }
    } else {
        if (offsetX > Globals::TILE_SIZE * 0.5) {
            tile += 1;
            offsetX -= Globals::TILE_SIZE;
            changeIndex = true;
        } else if (offsetX < -(Globals::TILE_SIZE * 0.5)) {
            tile -= 1;
            offsetX += Globals::TILE_SIZE;
            changeIndex = true;
        }

        if (offsetY > 2) {
            offsetY -= moveAmount;
        } else if (offsetY < -2) {
            offsetY += moveAmount;
        } else {
            offsetY = 0;
        }
    }

    if (changeIndex) {
        position = gameState.tileGrid[tile].GetPosition();
    }

    CalculateRect();
}

bool Player::MoveUp(GameState& gameState) {
    return Move(tile - Globals::TILE_ROWS, true, gameState);
}
bool Player::MoveDown(GameState& gameState) {
    return Move(tile + Globals::TILE_ROWS, true, gameState);
}
bool Player::MoveLeft(GameState& gameState) {
    return Move(tile - 1, false, gameState);
}
bool Player::MoveRight(GameState& gameState) {
    return Move(tile + 1, false, gameState);
}

bool Player::Move(int i, bool upDown, GameState& gameState) {
    if (tileExists(i)) {
        if (!tileHasCollider(gameState.tileGrid[i])) {
            return true;
        }
    }
    return false;
}

bool Player::tileExists(int i) {
    if (i >= 0 && i < Globals::TILE_COUNT) {
        return true;
    }
    return false;
}
bool Player::tileHasCollider(Tile& t) {
    if (t.GetState() == 0) {
        return false;
    }
    return true;
}
