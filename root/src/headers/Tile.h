//
// Copyright (c) 2016 by Ben Townshend. All Rights Reserved.
//

#ifndef tile_h
#define tile_h

#include "Globals.h"
#include "Sprite.h"
#include "Vector2.h"

/*
 * STATES
 * 0 - Empty
 * 1 - Left Wall
 * 2 - Top Wall
 * 3 - Right Wall
 * 4 - Bottom Wall
 * 5 - Bottom-Left Corner
 * 6 - Top-Left Corner
 * 7 - Top-Right Corner
 * 8 - Bottom-Right Corner
 */

class Tile {
public:
	void SetSprite(Sprite sprite) {
		this->sprite = sprite;
	}
	Sprite GetSprite() {
		return sprite;
	}
	void SetState(int state) {
		this->state = state;
	}
	int GetState() {
		return state;
	}
	int GetWidth() {
		return w;
	}
	int GetHeight() {
		return h;
	}

	// Constructor
	Tile(int index) {
		this->state = GRID_TYPE[index];
		this->w = index % 5;
		this->h = index % 2;
	}
private:
	Sprite sprite;
	int state;
	int w;
	int g;
};

#endif
