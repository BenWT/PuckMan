//
// Copyright (c) 2016 by Ben Townshend. All Rights Reserved.
//

#pragma once

//#include "Globals.h"
#include "GameState.h"
#include "Sprite.h"

class GameState;
class Player : public Sprite {
public:
	int tile;

	void MoveUp(GameState*);
	void MoveDown(GameState*);
	void MoveLeft(GameState*);
	void MoveRight(GameState*);
	using Sprite::Sprite; // inherit constructors

private:
	bool tileExists(int);
	bool firstInRow(int);
	bool lastInRow(int);
	bool shouldCollide(int);
};
