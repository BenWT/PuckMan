//
// Copyright (c) 2016 by Ben Townshend. All Rights Reserved.
//

#pragma once

#include "Globals.h"
#include "Sprite.h"
#include "Tile.h"

class GameState;
class Player : public Sprite {
public:
	int tile;
	double offsetX = 0;
	double offsetY = 0;

	bool CanMove(GameState&, MoveDirection);
	void DoMove(GameState&, bool, int, double);
	void Reset(double);
	using Sprite::Sprite; // inherit constructors
	void Render(SDL_Renderer* renderer); // overwrite Render

private:
	int getNextIndex(MoveDirection);
	bool tileExists(int);
	bool clampOffset(bool, GameState&);
	void returnToZero(double&, double);
};
