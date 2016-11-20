//
// Copyright (c) 2016 by Ben Townshend. All Rights Reserved.
//

#ifndef gamestate_h
#define gamestate_h

#include <array>
#include "Globals.h"
#include "Tile.h"
#include "Sprite.h"

enum MenuState { Main, Game };

class GameState {
public:
	// Variables
	Sprite playerSprite;
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

#endif
