//
// Copyright (c) 2016 by Ben Townshend. All Rights Reserved.
//

#pragma once

#include "Globals.h"
#include "Sprite.h"
#include "Vector2.h"

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
		this->h = (int)GRID_TYPE[index] / 5;
		this->w = GRID_TYPE[index] % 5;
	}
private:
	Sprite sprite;
	int state;
	int x;
	int y;
	int w;
	int h;
};
