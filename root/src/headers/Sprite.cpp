//
// Copyright (c) 2016 by Ben Townshend. All Rights Reserved.
//

#include "Sprite.h"
#include "Globals.h"

void Sprite::CalculateRect() {
    destRect.x = (int)(this->position->x - (this->scale->x / 2));
    destRect.y = (int)(this->position->y - (this->scale->y / 2));
    destRect.w = (int)(this->scale->x);
    destRect.h = (int)(this->scale->y);
}

void Sprite::SetTexture(SDL_Texture* texture) {
    this->texture = texture;
}

void Sprite::Render(SDL_Renderer* renderer) {
    SDL_RenderCopy(renderer, texture, (srcRect.w == -1) ? NULL : &srcRect, &destRect);
}

Sprite::Sprite() {}
Sprite::Sprite(SDL_Texture* texture, SDL_Rect srcRect, Vector2* position) {
    this->texture = texture;
    this->scale = new Vector2(Globals::TILE_SIZE, Globals::TILE_SIZE);
    this->position = position;
    this->srcRect = srcRect;

    this->position->x += (scale->x / 2);
    this->position->y += (scale->y / 2);

    CalculateRect();
}
Sprite::Sprite(SDL_Texture* texture, SDL_Rect srcRect, Vector2* position, Vector2* scale) {
    this->texture = texture;
    this->position = position;
    this->scale = scale;
    this->srcRect = srcRect;

    this->position->x += (this->scale->x / 2);
    this->position->y += (this->scale->y / 2);

    CalculateRect();
}
