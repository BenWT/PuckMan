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
	int GetX() {
		return x;
	}
	int GetY() {
		return y;
	}
	int GetWidth() {
		return w;
	}
	int GetHeight() {
		return h;
	}

	// Constructor
	Tile() {
		this->state = 0;
		this->x = 0;
		this->y = 0;
		this->w = 0;
		this->h = 0;
	}
	Tile(int index, int x, int y) {
		this->state = GRID_TYPE[index];
		this->x = x;
		this->y = y;
		/* this->w = GRID_TYPE[index] % 5;
		this->h = GRID_TYPE[index] % 2;

		if (GRID_TYPE[index] < 5) {
			this->w = GRID_TYPE[index];
			this->h = 0;
		} else {
			this->w = GRID_TYPE[index] ;
			this->h = 1;
		}*/

		this->h = (int)GRID_TYPE[index] / 5;
		this->w = GRID_TYPE[index] % 5;



		std::cout << this->state << " " << this->w << " " << this->h << std::endl;
	}
private:
	Sprite sprite;
	int state;
	int x;
	int y;
	int w;
	int h;
};

#endif
