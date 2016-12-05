//
// Copyright (c) 2016 by Ben Townshend. All Rights Reserved.
//

#include <iostream>
#include "GameState.h"
#include "Globals.h"

MenuState GameState::GetState() {
    return state;
}

void GameState::SetState(MenuState state) {
    this->state = state;
}

void GameState::IncreaseScore() {
    playerScore += 50;
    std::cout << "Score: " << playerScore << std::endl;
}

GameState::GameState() {
    this->playerScore = 0;
    this->state = Game;

	for (int y = 0; y < Globals::TILE_ROWS; y++) {
        for (int x = 0; x < Globals::TILE_ROWS; x++) {
            Tile* t = new Tile(x, y);
            tileGrid[x + (y * Globals::TILE_ROWS)] = *t;
            delete t;
        }
    }
}
