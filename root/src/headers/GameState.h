//
// Copyright (c) 2016 by Ben Townshend. All Rights Reserved.
//

#pragma once

#include <array>
#include <vector>
#include "Globals.h"
#include "Sprite.h"
#include "Player.h"
#include "Tile.h"
#include "FontSprite.h"

class GameState {
public:
	Player playerSprite;
	Player playerTwoSprite;
	std::array<Tile, Globals::TILE_COUNT> tileGrid;
	std::vector<FontSprite> mainMenuText;
	SDL_Texture* biscuitTexture;

	int mouseX = 0;
	int mouseY = 0;

	MenuState GetState();
	void SetState(MenuState);
	GameState();
private:
	MenuState state;
};
