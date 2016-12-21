//
// Copyright (c) 2016 by Ben Townshend. All Rights Reserved.
//

#pragma once

#include "SDL_mixer.h"
#include <array>
#include "Globals.h"
#include "Sprite.h"
#include "Player.h"
#include "Enemy.h"
#include "Tile.h"
#include "FontSprite.h"

class GameState {
public:
	Player playerSprite;
	Player playerTwoSprite;
	std::array<Tile, Globals::TILE_COUNT> tileGrid;
	std::vector<FontSprite> mainMenuText;
	std::vector<FontSprite> endGameOneText;
	std::vector<FontSprite> endGameTwoText;
	std::vector<FontSprite> optionsText;
	std::array<Enemy, 4> enemySprites;
	FontSprite playerScoreText;
	FontSprite playerTwoScoreText;
	SDL_Texture* biscuitTexture;
	SDL_Texture* pillTexture;
	SDL_Texture* heartTexture;
	double pillAngle = 0.0;

	// Audio
	Mix_Music* menuMusic = NULL;
	Mix_Music* gameMusic = NULL;
	Mix_Chunk* clickSound = NULL;
	Mix_Chunk* biscuitSound = NULL;
	Mix_Chunk* pillSound = NULL;
	Mix_Chunk* hitSound = NULL;
	Mix_Chunk* ghostDeathSound = NULL;
	void PlayClick();
	void PlayBiscuit();
	void PlayPill();
	void PlayHit();
	void PlayGhostDeath();

	// Options
	bool useController = false;
	double musicVolume = 0.5;
	SDL_Keycode playerOneUpKey;
	SDL_Keycode playerOneDownKey;
	SDL_Keycode playerOneLeftKey;
	SDL_Keycode playerOneRightKey;
	SDL_Keycode playerTwoUpKey;
	SDL_Keycode playerTwoDownKey;
	SDL_Keycode playerTwoLeftKey;
	SDL_Keycode playerTwoRightKey;
	SDL_Keycode selectKey;

	// Mouse
	bool mouseClicked = false;
	int mouseX = 0;
	int mouseY = 0;

	// Gamepad
	double leftJoystickX = 0;
	double leftJoystickY = 0;
	double rightJoystickX = 0;
	double rightJoystickY = 0;
	bool aGamePad = false;

	// Keyboard
	bool w = false;
	bool a = false;
	bool s = false;
	bool d = false;
	bool up = false;
	bool left = false;
	bool down = false;
	bool right = false;
	bool enter = false;

	// Menu
	bool paused = false;
	int mainMenuSelectionIndex = 1;
	int endGameOneSelectionIndex = 1;
	int endGameTwoSelectionIndex = 1;
	int optionsSelectionIndex = 2;
	double joystickSwapTime = 0.25;
	double joystickTimer = 0.0;

	void ResetInputs();
	MenuState GetState();
	void SetState(MenuState);
	GameState();
private:
	MenuState state;
};
