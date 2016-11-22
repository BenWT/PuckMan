//
// Copyright (c) 2016 by Ben Townshend. All Rights Reserved.
//

#pragma once

#include "Sprite.h"

// TODO Implement GameState linking
class GameState;
class Player : public Sprite {
public:
	int tile;

	void MoveUp();
	void MoveDown();
	void MoveLeft();
	void MoveRight();
	using Sprite::Sprite; // inherit constructors

private:
	bool tileExists(int);
	bool firstInRow(int);
	bool lastInRow(int);
	bool shouldCollide(int);
};
