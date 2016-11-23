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

//TODO continue up to collider
void Player::DoMove(GameState& gameState, double moveAmount) {
    double offset = 0.25;
    bool shouldChange = false;

    if (gameState.playerMoveDirection == Up) {
        int i = tile - Globals::TILE_ROWS;

        if (tileExists(i)) {
            if (gameState.tileGrid[i].GetState() == 0) {
                offsetY -= moveAmount;
                returnToZero(offsetX, moveAmount);
                if (clampOffset(true, moveAmount)) shouldChange = true;
            }
        }
    } else if (gameState.playerMoveDirection == Down) {
        int i = tile + Globals::TILE_ROWS;

        if (tileExists(i)) {
            if (gameState.tileGrid[tile + Globals::TILE_ROWS].GetState() == 0) {
                offsetY += moveAmount;
                returnToZero(offsetX, moveAmount);
                if (clampOffset(true, moveAmount)) shouldChange = true;
            }
        }
    } else if (gameState.playerMoveDirection == Left) {
        int i = tile - 1;

        if (tileExists(i)) {
            if (gameState.tileGrid[tile - 1].GetState() == 0) {
                offsetX -= moveAmount;
                returnToZero(offsetY, moveAmount);
                if (clampOffset(false, moveAmount)) shouldChange = true;
            }
        }
    } else if (gameState.playerMoveDirection == Right) {
        int i = tile + 1;
        if (tileExists(i)) {
            if (gameState.tileGrid[tile + 1].GetState() == 0) {
                offsetX += moveAmount;
                returnToZero(offsetY, moveAmount);
                if (clampOffset(false, moveAmount)) shouldChange = true;
            }
        }
    }

    if (shouldChange) {
        position = gameState.tileGrid[tile].GetPosition();
        CalculateRect();
    }
}

bool Player::tileExists(int index) {
    if (index >= 0 && index < Globals::TILE_COUNT) {
        return true;
    }
    return false;
}

bool Player::clampOffset(bool upDown, double deltaTime) {
    double offsetValue = 0.75;

    if (upDown) {
        if (offsetY > Globals::TILE_SIZE * offsetValue) {
            tile += Globals::TILE_ROWS;
            offsetY -= Globals::TILE_SIZE;
            return true;
        } else if (offsetY < -(Globals::TILE_SIZE * offsetValue)) {
            tile -= Globals::TILE_ROWS;
            offsetY += Globals::TILE_SIZE;
            return true;
        }
    } else {
        if (offsetX > Globals::TILE_SIZE * offsetValue) {
            tile++;
            offsetX -= Globals::TILE_SIZE;
            return true;
        } else if (offsetX < -(Globals::TILE_SIZE * offsetValue)) {
            tile--;
            offsetX += Globals::TILE_SIZE;
            return true;
        }
    }
    return false;
}

// TODO returnToZero needs smoothing
double Player::returnToZero(double& value, double deltaTime) {
    if (value == 0.0) {
        return 0.0;
    } else if (value > 0.0) {
        value -= deltaTime;
        if (value < 0.0) return 0.0;
    } else if (value < 0.0) {
        value += deltaTime;
        if (value > 0.0) return 0.0;
    }
    return value;
}
