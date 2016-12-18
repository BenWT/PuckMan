//
// Copyright (c) 2016 by Ben Townshend. All Rights Reserved.
//

#include <iostream>
#include "Sprite.h"
#include "Globals.h"

void Sprite::CalculateRect() {
    destRect.x = (int)(this->position->x);
    destRect.y = (int)(this->position->y);
    destRect.w = (int)(this->scale->x);
    destRect.h = (int)(this->scale->y);
}

void Sprite::SetTexture(SDL_Texture* texture) {
    this->texture = texture;
}

void Sprite::CycleAnims(double deltaTime) {
    if (doAnimate) {
        if (animCounter >= frameTime) {
            currentAnim++;
            animCounter = 0.0;
        }

        if (currentAnim >= animStates.size()) currentAnim = 0;
        animCounter += deltaTime;
    }
}

void Sprite::Render(SDL_Renderer* renderer) {
    if (doAnimate) SDL_RenderCopy(renderer, texture, &animStates.at(currentAnim), &destRect);
    else SDL_RenderCopy(renderer, texture, (srcRect.w == -1) ? NULL : &srcRect, &destRect);
}

Sprite::Sprite() {}
Sprite::Sprite(SDL_Texture* texture, SDL_Rect srcRect, Vector2* position) {
    this->texture = texture;
    this->scale = new Vector2(Globals::TILE_SIZE, Globals::TILE_SIZE);
    this->position = position;
    this->srcRect = srcRect;

    CalculateRect();
}
Sprite::Sprite(SDL_Texture* texture, SDL_Rect srcRect, Vector2* position, Vector2* scale) {
    this->texture = texture;
    this->position = position;
    this->scale = scale;
    this->srcRect = srcRect;

    CalculateRect();
}
