//
// Copyright (c) 2016 by Ben Townshend. All Rights Reserved.
//

#include "GameState.h"
#include "Globals.h"

MenuState GameState::GetState() {
    return state;
}

void GameState::SetState(MenuState state) {
    this->state = state;
}

GameState::GameState() {
    this->state = Game;

	for (int y = 0; y < Globals::TILE_ROWS; y++) {
        for (int x = 0; x < Globals::TILE_ROWS; x++) {
            Tile* t = new Tile(x + (y * Globals::TILE_ROWS), x, y);
            tileGrid[x + (y * Globals::TILE_ROWS)] = *t;
            delete t;
        }
    }
}
