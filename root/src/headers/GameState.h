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

	// Get + Set State
	MenuState GetState() {
		return state;
	}
	void SetState(MenuState state) {
		this->state = state;
	}

	// Get Tile
	Tile GetTile(int i) {
		return tileGrid[i];
	}
	Tile GetTile(int x, int y) {
		return tileGrid[y + (x * N_TILES)];
	}

	// Constructors
	GameState() {
		for (int i = 0; i < N_TILES; i++) {
			for (int j = 0; j < N_TILES; j++) {
				Tile *t = new Tile(j + (i * N_TILES));
				tileGrid[j + (i * N_TILES)] = *t;
				delete t;
			}
		}
	}
private:
	MenuState state;
	std::array<Tile, N_TILES * N_TILES> tileGrid;
};

#endif
