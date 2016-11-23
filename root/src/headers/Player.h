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

	void DoMove(GameState&, double);
	bool MoveUp(GameState&);
	bool MoveDown(GameState&);
	bool MoveLeft(GameState&);
	bool MoveRight(GameState&);
	using Sprite::Sprite; // inherit constructors
	void Render(SDL_Renderer* renderer); // overwrite Render

private:
	double offsetX = 0;
	double offsetY = 0;
	bool Move(int, bool, GameState&);
	bool tileExists(int);
	bool firstInRow(int);
	bool lastInRow(int);
	bool tileHasCollider(Tile&);
};
