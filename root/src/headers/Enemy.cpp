//
// Copyright (c) 2016 by Ben Townshend. All Rights Reserved.
//

#include <iostream>
#include "Globals.h"
#include "GameState.h"
#include "Tile.h"
#include "Enemy.h"

void Enemy::PathFind(GameState& gameState, double deltaTime) {
    if (timer >= 0.75) {
        if (moveDirection == Left) {
            if (toggle >= 2) {
                if (CanMove(gameState, Up)) { toggle++; timer = 0; moveDirection = Up; }
                else if (CanMove(gameState, Down)) { toggle++; timer = 0; moveDirection = Down; }
                else if (!CanMove(gameState, Left)) { toggle++; timer = 0; moveDirection = Right; }
                toggle = 0;
            } else {
                if (CanMove(gameState, Down)) { toggle++; timer = 0; moveDirection = Down; }
                else if (CanMove(gameState, Up)) { toggle++; timer = 0; moveDirection = Up; }
                else if (!CanMove(gameState, Left)) { toggle++; timer = 0; moveDirection = Right; }
            }
        } else if (moveDirection == Right) {
            if (toggle >= 2) {
                if (CanMove(gameState, Down)) { toggle++; timer = 0; moveDirection = Down; }
                else if (CanMove(gameState, Up)) { toggle++; timer = 0; moveDirection = Up; }
                else if (!CanMove(gameState, Right)) { toggle++; timer = 0; moveDirection = Left; }
                toggle = 0;
            } else {
                if (CanMove(gameState, Up)) { toggle++; timer = 0; moveDirection = Up; }
                else if (CanMove(gameState, Down)) { toggle++; timer = 0; moveDirection = Down; }
                else if (!CanMove(gameState, Right)) { toggle++; timer = 0; moveDirection = Left; }
            }
        } else if (moveDirection == Up) {
            if (toggle >= 2) {
                if (CanMove(gameState, Left)) { toggle++; timer = 0; moveDirection = Left; }
                else if (CanMove(gameState, Right)) { toggle++; timer = 0; moveDirection = Right; }
                else if (!CanMove(gameState, Up)) { toggle++; timer = 0; moveDirection = Down; }
                toggle = 0;
            } else {
                if (CanMove(gameState, Right)) { toggle++; timer = 0; moveDirection = Right; }
                else if (CanMove(gameState, Left)) { toggle++; timer = 0; moveDirection = Left; }
                else if (!CanMove(gameState, Up)) { toggle++; timer = 0; moveDirection = Down; }
            }
        } else if (moveDirection == Down) {
            if (toggle >= 2) {
                if (CanMove(gameState, Right)) { toggle++; timer = 0; moveDirection = Right; }
                else if (CanMove(gameState, Left)) { toggle++; timer = 0; moveDirection = Left; }
                else if (!CanMove(gameState, Down)) { toggle++; timer = 0; moveDirection = Up; }
                toggle = 0;
            } else {
                if (CanMove(gameState, Left)) { toggle++; timer = 0; moveDirection = Left; }
                else if (CanMove(gameState, Right)) { toggle++; timer = 0; moveDirection = Right; }
                else if (!CanMove(gameState, Down)) { toggle++; timer = 0; moveDirection = Up; }
            }
        }
    }

    DoMove(gameState, deltaTime * Globals::PLAYER_SPEED);
    timer += deltaTime;
}

void Enemy::Render(SDL_Renderer* renderer) {
    SDL_Rect drawRect = destRect;
    drawRect.x += (int)offsetX;
    drawRect.y += (int)offsetY;
    SDL_RenderCopy(renderer, texture, (srcRect.w == -1) ? NULL : &srcRect, &drawRect);
}
