//
// Copyright (c) 2016 by Ben Townshend. All Rights Reserved.
//

#pragma once

#include <array>
#include "Globals.h"
#include "Player.h"
#include "Sprite.h"
#include "Tile.h"

enum MenuState { Main, Game };

class GameState {
public:
	Player playerSprite;
	std::array<Tile, Globals::TILE_COUNT> tileGrid;

	MenuState GetState();
	void SetState(MenuState);
	GameState();
private:
	MenuState state;
};
