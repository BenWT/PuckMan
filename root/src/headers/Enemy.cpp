//
// Copyright (c) 2016 by Ben Townshend. All Rights Reserved.
//

#include <iostream>
#include <cstdlib>
#include "Globals.h"
#include "GameState.h"
#include "Tile.h"
#include "Enemy.h"

void Enemy::Kill(GameState& gameState) {
    this->tile = Globals::ENEMY_START_X[0] + (Globals::ENEMY_START_Y[0] * Globals::TILE_ROWS);
    this->SetPositionFromTile(gameState);
    this->moveDirection = Up;
}

MoveDirection Enemy::TurnLeft() {
    switch (moveDirection) {
        case Left: return Down; break;
        case Right: return Up; break;
        case Up: return Left; break;
        case Down: return Right; break;
        default: return moveDirection; break;
    }
}
MoveDirection Enemy::TurnRight() {
    switch (moveDirection) {
        case Left: return Up; break;
        case Right: return Down; break;
        case Up: return Right; break;
        case Down: return Left; break;
        default: return moveDirection; break;
    }
}
MoveDirection Enemy::TurnAround() {
    switch (moveDirection) {
        case Left: return Right; break;
        case Right: return Left; break;
        case Up: return Down; break;
        case Down: return Up; break;
        default: return moveDirection; break;
    }
}

void Enemy::Roam(GameState& gameState, double deltaTime) {
    deathTimer = 10.0;
    if (timer >= 0.25) {
        bool forward = CanMove(gameState, moveDirection, deltaTime);
        bool left = CanMove(gameState, TurnLeft(), deltaTime);
        bool right = CanMove(gameState, TurnRight(), deltaTime);
        int choice = rand();

        if (forward) {
            if (left) {
                if (right) { // left + right + forward
                    if (choice % 3 == 0) moveDirection = TurnLeft();
                    else if (choice % 3 == 1) moveDirection = TurnRight();
                    else if (choice % 3 == 2) // leave move direction

                    timer = 0;
                } else { // left + forward
                    if (choice % 2 == 0) moveDirection = TurnLeft();
                    else if (choice % 2 == 1) // leave move direction

                    timer = 0;
                }
            } else {
                if (right) { // right + forward
                    if (choice % 2 == 0) moveDirection = TurnRight();
                    else if (choice % 2 == 1) // leave move direction

                    timer = 0;
                } else { // forward
                    timer = 0;
                }
            }
        } else {
            if (left) {
                if (right) { // left + right
                    if (choice % 2 == 0) moveDirection = TurnLeft();
                    else if (choice % 2 == 1) moveDirection = TurnRight();

                    timer = 0;
                } else { // left
                    moveDirection = TurnLeft();
                    timer = 0;
                }
            } else {
                if (right) { // right
                    moveDirection = TurnRight();
                    timer = 0;
                } else { // turn around
                    moveDirection = TurnAround();
                    timer = 0;
                }
            }
        }
    }

    if (gameState.GetState() == TwoPlayer) {
        if (PlayerTwoOnRow(gameState)) {
            if (tile < gameState.playerTwoSprite.tile) moveDirection = Right;
            else moveDirection = Left;
        }
        else if (PlayerTwoOnColumn(gameState)) {
            if (tile < gameState.playerTwoSprite.tile) moveDirection = Down;
            else moveDirection = Up;
        }
    }

    if (PlayerOneOnRow(gameState)) {
        if (tile < gameState.playerSprite.tile) moveDirection = Right;
        else moveDirection = Left;
    }
    else if (PlayerOneOnColumn(gameState)) {
        if (tile < gameState.playerSprite.tile) moveDirection = Down;
        else moveDirection = Up;
    }

    DoMove(gameState, deltaTime * Globals::PLAYER_SPEED, deltaTime);
    timer += deltaTime;
}

bool Enemy::PlayerOneOnRow(GameState& gameState) {
    if (gameState.playerSprite.tile / Globals::TILE_ROWS == tile / Globals::TILE_ROWS) {
        if (tile < gameState.playerSprite.tile) {
            for (int i = tile; i < gameState.playerSprite.tile; i++) {
                if (gameState.tileGrid[i].GetState() > 0) return false;
            }
            return true;
        } else if (tile > gameState.playerSprite.tile) {
            for (int i = gameState.playerSprite.tile; i < tile; i++) {
                if (gameState.tileGrid[i].GetState() > 0) return false;
            }
            return true;
        }
    }
    return false;
}
bool Enemy::PlayerTwoOnRow(GameState& gameState) {
    if (gameState.playerTwoSprite.tile / Globals::TILE_ROWS == tile / Globals::TILE_ROWS) {
        if (tile < gameState.playerTwoSprite.tile) {
            for (int i = tile; i < gameState.playerTwoSprite.tile; i++) {
                if (gameState.tileGrid[i].GetState() > 0) return false;
            }
            return true;
        } else if (tile > gameState.playerTwoSprite.tile) {
            for (int i = gameState.playerTwoSprite.tile; i < tile; i++) {
                if (gameState.tileGrid[i].GetState() > 0) return false;
            }
            return true;
        }
    }
    return false;
}

bool Enemy::PlayerOneOnColumn(GameState& gameState) {
    if (gameState.playerSprite.tile % Globals::TILE_ROWS == tile % Globals::TILE_ROWS) {
        if (tile < gameState.playerSprite.tile) {
            for (int i = tile; i < gameState.playerSprite.tile; i += Globals::TILE_ROWS) {
                if (gameState.tileGrid[i].GetState() > 0) return false;
            }
            return true;
        } else if (tile > gameState.playerSprite.tile) {
            for (int i = gameState.playerSprite.tile; i < tile; i += Globals::TILE_ROWS) {
                if (gameState.tileGrid[i].GetState() > 0) return false;
            }
            return true;
        }
    }
    return false;
}
bool Enemy::PlayerTwoOnColumn(GameState& gameState) {
    if (gameState.playerTwoSprite.tile % Globals::TILE_ROWS == tile % Globals::TILE_ROWS) {
        if (tile < gameState.playerTwoSprite.tile) {
            for (int i = tile; i < gameState.playerTwoSprite.tile; i += Globals::TILE_ROWS) {
                if (gameState.tileGrid[i].GetState() > 0) return false;
            }
            return true;
        } else if (tile > gameState.playerTwoSprite.tile) {
            for (int i = gameState.playerTwoSprite.tile; i < tile; i += Globals::TILE_ROWS) {
                if (gameState.tileGrid[i].GetState() > 0) return false;
            }
            return true;
        }
    }
    return false;
}

void Enemy::Render(SDL_Renderer* renderer, bool hasPill) {
    SDL_Rect drawRect = destRect;
    drawRect.x += (int)offsetX;
    drawRect.y += (int)offsetY;

    if (hasPill) {
        if (doAnimate)  {
            SDL_Rect src;
            src = animStates.at(currentAnim);
            src.y = 400;

            SDL_RenderCopy(renderer,texture, &src, &drawRect);
        }
        else SDL_RenderCopy(renderer, texture, (srcRect.w == -1) ? NULL : &srcRect, &drawRect);
    } else {
        if (doAnimate) SDL_RenderCopy(renderer,texture, &animStates.at(currentAnim), &drawRect);
        else SDL_RenderCopy(renderer, texture, (srcRect.w == -1) ? NULL : &srcRect, &drawRect);
    }
}
