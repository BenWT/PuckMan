//
// Copyright (c) 2016 by Ben Townshend. All Rights Reserved.
//

#pragma once

#include "Globals.h"
#include "Sprite.h"
#include "Tile.h"

class Enemy : public Sprite {
public:
    MoveDirection moveDirection;
    int tile;
    double offsetX = 0;
    double offsetY = 0;

    void Render(SDL_Renderer* renderer); // overwrite Render
    void SetPositionFromTile(GameState&);
    using Sprite::Sprite;
};
