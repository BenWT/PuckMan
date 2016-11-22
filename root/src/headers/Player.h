//
// Copyright (c) 2016 by Ben Townshend. All Rights Reserved.
//

#pragma once

#include "Globals.h"
#include "GameState.h"
#include "Sprite.h"

class Player : public Sprite {
public:
	int tile;
	using Sprite::Sprite;

	bool tileExists(int index) {
		if (index >= 0 && index < N_TILES * N_TILES) {
			return true;
		}
		return false;
	}
	bool firstInRow(int index) {
		if ((index) % N_TILES == 0) {
			return true;
		}
		return false;
	}
	bool lastInRow(int index) {
		if ((index + 1) % N_TILES == 0) {
			return true;
		}
		return false;
	}
	bool shouldCollide(int newIndex) {
		/* if (gameState) {

		}*/
	}

	void MoveUp() {
		if (tileExists(tile - N_TILES)) {
			std::cout << "MoveUp()" << std::endl;

			tile = tile - N_TILES;
			position.y -= TILE_SIZE;
			CalculateRect();
		}
	}
	void MoveDown() {
		if (tileExists(tile + N_TILES)) {
			std::cout << "MoveDown()" << std::endl;

			tile = tile + N_TILES;
			position.y += TILE_SIZE;
			CalculateRect();
		}
	}
	void MoveLeft() {
		if (!firstInRow(tile)) {
			std::cout << "MoveLeft()" << std::endl;

			tile = tile - 1;
			position.x -= TILE_SIZE;
			CalculateRect();
		}
	}
	void MoveRight() {
		if (!lastInRow(tile)) {
			std::cout << "MoveRight()" << std::endl;

			tile = tile + 1;
			position.x += TILE_SIZE;
			CalculateRect();
		}
	}

};
