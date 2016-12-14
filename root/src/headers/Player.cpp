//
// Copyright (c) 2016 by Ben Townshend. All Rights Reserved.
//

#include <iostream>

#include "Globals.h"
#include "GameState.h"
#include "Tile.h"
#include "Player.h"

void Player::Render(SDL_Renderer* renderer) {
    if (alive)  {
        SDL_Rect drawRect = destRect;
        drawRect.x += (int)offsetX;
        drawRect.y += (int)offsetY;
        SDL_RenderCopy(renderer, texture, (srcRect.w == -1) ? NULL : &srcRect, &drawRect);
    }
}

bool Player::CanMove(GameState& gameState, MoveDirection direction) {
    int i = getNextIndex(direction);

    if (tileExists(i)) {
        int s = gameState.tileGrid[i].GetState();
        if (s == 0 || s == -1) {
            return true;
        }
    }
    return false;
}

void Player::DoMove(GameState& gameState, double moveAmount) {
    int i = getNextIndex(moveDirection);

    if (tileExists(i)) {
        int state = gameState.tileGrid[i].GetState();
        if (state == 0 || state == -1) {
			if (moveDirection == Up) {
				offsetY -= moveAmount;
				returnToZero(offsetX, moveAmount);
			} else if (moveDirection == Down) {
				offsetY += moveAmount;
				returnToZero(offsetX, moveAmount);
			} else if (moveDirection == Left) {
				offsetX -= moveAmount;
				returnToZero(offsetY, moveAmount);
			} else if (moveDirection == Right) {
				offsetX += moveAmount;
				returnToZero(offsetY, moveAmount);
			}

            if (clampOffset(gameState)) {
                SetPositionFromTile(gameState);
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

void Player::SetPositionFromTile(GameState& gameState) {
	position = gameState.tileGrid[tile].GetPosition();
	CalculateRect();
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

bool Player::clampOffset(GameState& gameState) {
	double offsetValue = 0.50;
    bool hasChanged = false;
	bool upDown = true;
	if (moveDirection == Left || moveDirection == Right) upDown = false;

    if (upDown) {
        if (offsetY > Globals::TILE_SIZE * offsetValue) {
            tile += Globals::TILE_ROWS;
            offsetY -= Globals::TILE_SIZE;
            hasChanged = true;
        } else if (offsetY < -(Globals::TILE_SIZE * offsetValue)) {
            tile -= Globals::TILE_ROWS;
            offsetY += Globals::TILE_SIZE;
            hasChanged = true;
        }
    } else {
        if (offsetX > Globals::TILE_SIZE * offsetValue) {
            tile++;
            offsetX -= Globals::TILE_SIZE;
            hasChanged = true;
        } else if (offsetX < -(Globals::TILE_SIZE * offsetValue)) {
            tile--;
            offsetX += Globals::TILE_SIZE;
            hasChanged = true;
        }
    }

    if (hasChanged) {
        if (gameState.tileGrid[tile].CheckBiscuit()) {
            gameState.tileGrid[tile].EatBiscuit();
            score += Globals::BISCUIT_SCORE;
        }
        return true;
    }

    return false;
}

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
