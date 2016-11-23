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
    bool shouldChange = false;

    if (gameState.playerMoveDirection == Up) {
        if ((tile - Globals::TILE_ROWS) >= 0) {
            if (gameState.tileGrid[tile - Globals::TILE_ROWS].GetState() == 0) {
                offsetY -= moveAmount;
                offsetX = 0;

                if (offsetY > 0.25 * Globals::TILE_SIZE) {
                    tile += Globals::TILE_ROWS;
                    offsetY -= Globals::TILE_SIZE;
                    shouldChange = true;
                } else if (offsetY < -(0.25 * Globals::TILE_SIZE)) {
                    tile -= Globals::TILE_ROWS;
                    offsetY += Globals::TILE_SIZE;
                    shouldChange = true;
                }
            } else {
                offsetX = 0;
                offsetY = 0;
            }
        }
    } else if (gameState.playerMoveDirection == Down) {
        if ((tile + Globals::TILE_ROWS) >= 0) {
            if (gameState.tileGrid[tile + Globals::TILE_ROWS].GetState() == 0) {
                offsetY += moveAmount;
                offsetX = 0;

                if (offsetY > 0.25 * Globals::TILE_SIZE) {
                    tile += Globals::TILE_ROWS;
                    offsetY -= Globals::TILE_SIZE;
                    shouldChange = true;
                } else if (offsetY < -(0.25 * Globals::TILE_SIZE)) {
                    tile -= Globals::TILE_ROWS;
                    offsetY += Globals::TILE_SIZE;
                    shouldChange = true;
                }
            } else {
                offsetX = 0;
                offsetY = 0;
            }
        }
    } else if (gameState.playerMoveDirection == Left) {
        if (gameState.tileGrid[tile - 1].GetState() == 0) {
            offsetX -= moveAmount;
            offsetY = 0;

            if (offsetX > 0.25 * Globals::TILE_SIZE) {
                tile += 1;
                offsetX -= Globals::TILE_SIZE;
                shouldChange = true;
            } else if (offsetX < -(0.25 * Globals::TILE_SIZE)) {
                tile -= 1;
                offsetX += Globals::TILE_SIZE;
                shouldChange = true;
            }
        } else {
            offsetX = 0;
            offsetY = 0;
        }
    } else if (gameState.playerMoveDirection == Right) {
        if (gameState.tileGrid[tile + 1].GetState() == 0) {
            offsetX += moveAmount;
            offsetY = 0;

            if (offsetX > 0.25 * Globals::TILE_SIZE) {
                tile += 1;
                offsetX -= Globals::TILE_SIZE;
                shouldChange = true;
            } else if (offsetX < -(0.25 * Globals::TILE_SIZE)) {
                tile -= 1;
                offsetX += Globals::TILE_SIZE;
                shouldChange = true;
            }
        } else {
            offsetX = 0;
            offsetY = 0;
        }
    }

    if (shouldChange) {
        position = gameState.tileGrid[tile].GetPosition();
        CalculateRect();
    }
}
