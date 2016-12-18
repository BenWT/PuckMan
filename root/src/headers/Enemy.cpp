//
// Copyright (c) 2016 by Ben Townshend. All Rights Reserved.
//

#include <iostream>
#include <cstdlib>
#include "Globals.h"
#include "GameState.h"
#include "Tile.h"
#include "Enemy.h"

void Enemy::PathFind(GameState& gameState, double deltaTime) {
    if (timer >= 0.15) {
        if (moveDirection == Left) {
            if (currentToggle >= toggle) {
                if (CanMove(gameState, Up)) { currentToggle++; timer = 0; moveDirection = Up; }
                else if (CanMove(gameState, Down)) { currentToggle++; timer = 0; moveDirection = Down; }
                else if (!CanMove(gameState, Left)) { currentToggle++; timer = 0; moveDirection = Right; }
                currentToggle = 0;
                toggle = rand() % 10;
            } else {
                if (CanMove(gameState, Down)) { currentToggle++; timer = 0; moveDirection = Down; }
                else if (CanMove(gameState, Up)) { currentToggle++; timer = 0; moveDirection = Up; }
                else if (!CanMove(gameState, Left)) { currentToggle++; timer = 0; moveDirection = Right; }
            }
        } else if (moveDirection == Right) {
            if (currentToggle >= toggle) {
                if (CanMove(gameState, Down)) { currentToggle++; timer = 0; moveDirection = Down; }
                else if (CanMove(gameState, Up)) { currentToggle++; timer = 0; moveDirection = Up; }
                else if (!CanMove(gameState, Right)) { currentToggle++; timer = 0; moveDirection = Left; }
                currentToggle = 0;
                toggle = rand() % 10;
            } else {
                if (CanMove(gameState, Up)) { currentToggle++; timer = 0; moveDirection = Up; }
                else if (CanMove(gameState, Down)) { currentToggle++; timer = 0; moveDirection = Down; }
                else if (!CanMove(gameState, Right)) { currentToggle++; timer = 0; moveDirection = Left; }
            }
        } else if (moveDirection == Up) {
            if (currentToggle >= toggle) {
                if (CanMove(gameState, Left)) { currentToggle++; timer = 0; moveDirection = Left; }
                else if (CanMove(gameState, Right)) { currentToggle++; timer = 0; moveDirection = Right; }
                else if (!CanMove(gameState, Up)) { currentToggle++; timer = 0; moveDirection = Down; }
                currentToggle = 0;
                toggle = rand() % 10;
            } else {
                if (CanMove(gameState, Right)) { currentToggle++; timer = 0; moveDirection = Right; }
                else if (CanMove(gameState, Left)) { currentToggle++; timer = 0; moveDirection = Left; }
                else if (!CanMove(gameState, Up)) { currentToggle++; timer = 0; moveDirection = Down; }
            }
        } else if (moveDirection == Down) {
            if (currentToggle >= toggle) {
                if (CanMove(gameState, Right)) { currentToggle++; timer = 0; moveDirection = Right; }
                else if (CanMove(gameState, Left)) { currentToggle++; timer = 0; moveDirection = Left; }
                else if (!CanMove(gameState, Down)) { currentToggle++; timer = 0; moveDirection = Up; }
                currentToggle = 0;
                toggle = rand() % 10;
            } else {
                if (CanMove(gameState, Left)) { currentToggle++; timer = 0; moveDirection = Left; }
                else if (CanMove(gameState, Right)) { currentToggle++; timer = 0; moveDirection = Right; }
                else if (!CanMove(gameState, Down)) { currentToggle++; timer = 0; moveDirection = Up; }
            }
        }
    }

    DoMove(gameState, deltaTime * Globals::PLAYER_SPEED, deltaTime);
    timer += deltaTime;
}

void Enemy::Render(SDL_Renderer* renderer) {
    SDL_Rect drawRect = destRect;
    drawRect.x += (int)offsetX;
    drawRect.y += (int)offsetY;

    if (doAnimate) SDL_RenderCopy(renderer,texture, &animStates.at(currentAnim), &drawRect);
    else SDL_RenderCopy(renderer, texture, (srcRect.w == -1) ? NULL : &srcRect, &drawRect);
}
