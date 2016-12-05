//
// Copyright (c) 2016 by Ben Townshend. All Rights Reserved.
//

#include "Globals.h"
#include "Tile.h"

void Tile::SetSprite(Sprite sprite) {
    this->sprite = sprite;
}
Sprite Tile::GetSprite() {
    return sprite;
}

void Tile::SetState(int state) {
    this->state = state;
}
int Tile::GetState() {
    return state;
}

bool Tile::CheckBiscuit() {
    return hasBiscuit;
}
void Tile::EatBiscuit() {
    hasBiscuit = false;
}

Vector2* Tile::GetPosition() {
    return position;
}
Vector2* Tile::GetTexturePosition() {
    return texturePosition;
}
void Tile::SetTexturePosition(int index) {
    if (Globals::GRID_TYPE[index] == -1) {
        index = 0;
        this->hasBiscuit = true;
        this->texturePosition = new Vector2(0, 0);
    } else {
        this->texturePosition = new Vector2((Globals::GRID_TYPE[index] % 5) * Globals::TILE_SIZE, ((int)Globals::GRID_TYPE[index] / 5) * Globals::TILE_SIZE);
    }
}
double Tile::GetPositionX() {
    return position->x;
}
double Tile::GetPositionY() {
    return position->y;
}
double Tile::GetTextureX() {
    return texturePosition->x;
}
double Tile::GetTextureY() {
    return texturePosition->y;
}

Tile::Tile() {
    this->state = 0;
    this->position = new Vector2(0, 0);
    this->hasBiscuit = false;
    this->texturePosition = new Vector2(0, 0);
}
Tile::Tile(int x, int y) {
    int index = x + (y * Globals::TILE_ROWS);
    this->state = Globals::GRID_TYPE[index];
    this->position = new Vector2(x * Globals::TILE_SIZE, y * Globals::TILE_SIZE);
    this->hasBiscuit = false;
    SetTexturePosition(index);
}
