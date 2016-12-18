//
// Copyright (c) 2016 by Ben Townshend. All Rights Reserved.
//

#pragma once

#include <cstdlib>
#include "Globals.h"
#include "Sprite.h"
#include "Tile.h"

class Enemy : public Player {
public:
    void PathFind(GameState&);

    double timer = 1;

    void PathFind(GameState&, double);
    void Render(SDL_Renderer* renderer); // overwrite Render
    using Player::Player;
private:
    int toggle = rand() % 10;
    int currentToggle = 0;
};
