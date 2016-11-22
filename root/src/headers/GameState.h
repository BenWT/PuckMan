//
// Copyright (c) 2016 by Ben Townshend. All Rights Reserved.
//

#pragma once

#include <array>
#include "Globals.h"
#include "Tile.h"
#include "Sprite.h"
#include "Player.h"

enum MenuState { Main, Game };

class GameState {
public:
	// Variables
	Player playerSprite;
	std::array<Tile, N_TILES * N_TILES> tileGrid;

	// Get + Set State
	MenuState GetState() {
		return state;
	}
	void SetState(MenuState state) {
		this->state = state;
	}

	// Constructors
	GameState() {
		this->state = Game;

		for (int y = 0; y < N_TILES; y++) {
			for (int x = 0; x < N_TILES; x++) {
				Tile *t = new Tile(x + (y * N_TILES), x, y);
				tileGrid[x + (y * N_TILES)] = *t;
				delete t;
			}
		}
	}
private:
	MenuState state;
};
