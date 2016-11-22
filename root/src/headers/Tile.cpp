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

int Tile::GetX() {
    return position->x;
}
int Tile::GetY() {
    return position->y;
}
int Tile::GetWidth() {
    return scale->x;
}
int Tile::GetHeight() {
    return scale->y;
}

Tile::Tile() {
    this->state = 0;
    this->position = new Vector2(0, 0);
    this->scale = new Vector2(0, 0);
}
Tile::Tile(int index, int x, int y) {
    this->state = Globals::GRID_TYPE[index];
    this->position = new Vector2(x, y);
    this->scale = new Vector2((int)Globals::GRID_TYPE[index] / 5, Globals::GRID_TYPE[index] % 5);
}
