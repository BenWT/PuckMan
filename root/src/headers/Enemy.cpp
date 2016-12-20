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
    if (timer >= 0.15) {
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


    DoMove(gameState, deltaTime * Globals::PLAYER_SPEED, deltaTime);
    timer += deltaTime;

    /* if (timer >= 0.15) {
        if (moveDirection == Left) {
            if (currentToggle >= toggle) {
                if (CanMove(gameState, Up, deltaTime)) { currentToggle++; timer = 0; moveDirection = Up; }
                else if (CanMove(gameState, Down, deltaTime)) { currentToggle++; timer = 0; moveDirection = Down; }
                else if (!CanMove(gameState, Left, deltaTime)) { currentToggle++; timer = 0; moveDirection = Right; }
                currentToggle = 0;
                toggle = rand() % 10;
            } else {
                if (CanMove(gameState, Down, deltaTime)) { currentToggle++; timer = 0; moveDirection = Down; }
                else if (CanMove(gameState, Up, deltaTime)) { currentToggle++; timer = 0; moveDirection = Up; }
                else if (!CanMove(gameState, Left, deltaTime)) { currentToggle++; timer = 0; moveDirection = Right; }
            }
        } else if (moveDirection == Right) {
            if (currentToggle >= toggle) {
                if (CanMove(gameState, Down, deltaTime)) { currentToggle++; timer = 0; moveDirection = Down; }
                else if (CanMove(gameState, Up, deltaTime)) { currentToggle++; timer = 0; moveDirection = Up; }
                else if (!CanMove(gameState, Right, deltaTime)) { currentToggle++; timer = 0; moveDirection = Left; }
                currentToggle = 0;
                toggle = rand() % 10;
            } else {
                if (CanMove(gameState, Up, deltaTime)) { currentToggle++; timer = 0; moveDirection = Up; }
                else if (CanMove(gameState, Down, deltaTime)) { currentToggle++; timer = 0; moveDirection = Down; }
                else if (!CanMove(gameState, Right, deltaTime)) { currentToggle++; timer = 0; moveDirection = Left; }
            }
        } else if (moveDirection == Up) {
            if (currentToggle >= toggle) {
                if (CanMove(gameState, Left, deltaTime)) { currentToggle++; timer = 0; moveDirection = Left; }
                else if (CanMove(gameState, Right, deltaTime)) { currentToggle++; timer = 0; moveDirection = Right; }
                else if (!CanMove(gameState, Up, deltaTime)) { currentToggle++; timer = 0; moveDirection = Down; }
                currentToggle = 0;
                toggle = rand() % 10;
            } else {
                if (CanMove(gameState, Right, deltaTime)) { currentToggle++; timer = 0; moveDirection = Right; }
                else if (CanMove(gameState, Left, deltaTime)) { currentToggle++; timer = 0; moveDirection = Left; }
                else if (!CanMove(gameState, Up, deltaTime)) { currentToggle++; timer = 0; moveDirection = Down; }
            }
        } else if (moveDirection == Down) {
            if (currentToggle >= toggle) {
                if (CanMove(gameState, Right, deltaTime)) { currentToggle++; timer = 0; moveDirection = Right; }
                else if (CanMove(gameState, Left, deltaTime)) { currentToggle++; timer = 0; moveDirection = Left; }
                else if (!CanMove(gameState, Down, deltaTime)) { currentToggle++; timer = 0; moveDirection = Up; }
                currentToggle = 0;
                toggle = rand() % 10;
            } else {
                if (CanMove(gameState, Left, deltaTime)) { currentToggle++; timer = 0; moveDirection = Left; }
                else if (CanMove(gameState, Right, deltaTime)) { currentToggle++; timer = 0; moveDirection = Right; }
                else if (!CanMove(gameState, Down, deltaTime)) { currentToggle++; timer = 0; moveDirection = Up; }
            }
        }
    }

    DoMove(gameState, deltaTime * Globals::PLAYER_SPEED, deltaTime);
    timer += deltaTime; */
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
