//
// Copyright (c) 2016 by Ben Townshend. All Rights Reserved.
//

#include "GameState.h"

MenuState GameState::GetState() {
    return state;
}

void GameState::SetState(MenuState state) {
    this->state = state;
}

GameState::GameState() {
    this->state = Game;

    for (int y = 0; y < TILE_ROWS; y++) {
        for (int x = 0; x < TILE_ROWS; x++) {
            Tile* t = new Tile(x + (y * TILE_ROWS), x, y);
            tileGrid[x + (y * TILE_ROWS)] = *t;
            delete t;
        }
    }
}
