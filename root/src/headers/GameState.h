//
// Copyright (c) 2016 by Ben Townshend. All Rights Reserved.
//

#pragma once

#include <array>
#include "Globals.h"
#include "Player.h"
#include "Sprite.h"
#include "Tile.h"

class GameState {
public:
	Player playerSprite;
	int playerScore;
	MoveDirection playerMoveDirection;
	std::array<Tile, Globals::TILE_COUNT> tileGrid;
	SDL_Texture* biscuitTexture;

	MenuState GetState();
	void SetState(MenuState);
	void IncreaseScore();
	GameState();
private:
	MenuState state;
};
