//
// Copyright (c) 2016 by Ben Townshend. All Rights Reserved.
//

#pragma once

#include "Sprite.h"
#include "Tile.h"

class GameState;
class Player : public Sprite {
public:
	int tile;
	double offsetX = 0;
	double offsetY = 0;

	void DoMove(GameState&, double);
	using Sprite::Sprite; // inherit constructors
	void Render(SDL_Renderer* renderer); // overwrite Render

private:
	bool tileExists(int);
	bool clampOffset(bool, double);
	double returnToZero(double&, double);
};
