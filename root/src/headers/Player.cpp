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

bool Player::CanMove(GameState& gameState, MoveDirection direction) {
    int i = getNextIndex(direction);

    if (tileExists(i)) {
        if (gameState.tileGrid[i].GetState() == 0) {
            return true;
        }
    }
    return false;
}

//TODO continue up to collider
void Player::DoMove(GameState& gameState, bool upDown, int direction, double moveAmount) {
    int i = getNextIndex(gameState.playerMoveDirection);

    if (tileExists(i)) {
        if (gameState.tileGrid[i].GetState() == 0) {
            if (upDown) {
                offsetY += moveAmount * direction;
                returnToZero(offsetX, moveAmount);
            } else {
                offsetX += moveAmount * direction;
                returnToZero(offsetY, moveAmount);
            }

            if (clampOffset(upDown)) {
                position = gameState.tileGrid[tile].GetPosition();
                CalculateRect();
            }
        } else {
            Reset(moveAmount);
        }
    }
}

void Player::Reset(double deltaTime) {
    returnToZero(offsetX, deltaTime);
    returnToZero(offsetY, deltaTime);
}

int Player::getNextIndex(MoveDirection direction) {
    int i = -1;

    switch (direction) {
        case 0:
            i = tile - Globals::TILE_ROWS;
            break;
        case 1:
            i = tile + Globals::TILE_ROWS;
            break;
        case 2:
            i = tile - 1;
            break;
        case 3:
            i = tile + 1;
            break;
        default:
            i = -1;
            break;
    }

    return i;
}

bool Player::tileExists(int index) {
    if (index >= 0 && index < Globals::TILE_COUNT) {
        return true;
    }
    return false;
}

bool Player::clampOffset(bool upDown) {
    double offsetValue = 0.5;

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
void Player::returnToZero(double& value, double deltaTime) {
    if (value == 0.0) {
        value = 0.0;
    } else if (value > 0.0) {
        value -= deltaTime;
        if (value < 0.0) value = 0.0;
    } else if (value < 0.0) {
        value += deltaTime;
        if (value > 0.0) value = 0.0;
    }
}
