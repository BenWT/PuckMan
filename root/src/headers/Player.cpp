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
        drawRect.x += (int)offsetX; drawRect.y += (int)offsetY;
        int angle = 0;

        switch (moveDirection) {
            case Left: angle = 0; break;
            case Up: angle = 90; break;
            case Right: angle = 180; break;
            case Down: angle = 270; break;
        }

        if (deathTimer <= deathTime) {
            SDL_Rect src;

            if (doAnimate) {
                src = animStates.at(currentAnim);
                src.y += 200;
                SDL_RenderCopyEx(renderer, texture, &src, &drawRect, angle, NULL, SDL_FLIP_NONE);
            } else {
                src = srcRect;
                src.y += 200;
                SDL_RenderCopyEx(renderer, texture, (srcRect.w == -1) ? NULL : &src, &drawRect, angle, NULL, SDL_FLIP_NONE);
            }
        } else {
            if (doAnimate) SDL_RenderCopyEx(renderer, texture, &animStates.at(currentAnim), &drawRect, angle, NULL, SDL_FLIP_NONE);
            else SDL_RenderCopyEx(renderer, texture, (srcRect.w == -1) ? NULL : &srcRect, &drawRect, angle, NULL, SDL_FLIP_NONE);
        }
    }
}

bool Player::CanMove(GameState& gameState, MoveDirection direction, double deltaTime) {
    int i = getNextIndex(direction);

    if (tileExists(i)) {
        int s = gameState.tileGrid[i].GetState();
        if (s == 0 || s == -1 || s == -2) {
            return true;
        }
    }
    return false;
}

void Player::DoMove(GameState& gameState, double moveAmount, double deltaTime) {
    int i = getNextIndex(moveDirection);

    if (hasPill) moveAmount *= 1.5;

    if (tileExists(i) && alive) {
        int state = gameState.tileGrid[i].GetState();
        if (state == 0 || state == -1 || state == -2) {
            CycleAnims(deltaTime);

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
        if (canBiscuit) {
            if (gameState.tileGrid[tile].CheckBiscuit()) {
                gameState.tileGrid[tile].EatBiscuit();
                gameState.PlayBiscuit();
                score += Globals::BISCUIT_SCORE;
            }
            if (gameState.tileGrid[tile].CheckPill()) {
                gameState.tileGrid[tile].EatPill();
                gameState.PlayPill();
                this->pillTimer = 0.0;
                this->hasPill = true;
            }
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
