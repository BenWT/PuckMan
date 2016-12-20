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

    void Kill(GameState&);
    void Roam(GameState&, double);
    MoveDirection TurnLeft();
    MoveDirection TurnRight();
    MoveDirection TurnAround();
    void Render(SDL_Renderer* renderer, bool); // overwrite Render
    using Player::Player;
private:
    int toggle = rand() % 10;
    int currentToggle = 0;
};
