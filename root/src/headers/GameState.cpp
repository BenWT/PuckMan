//
// Copyright (c) 2016 by Ben Townshend. All Rights Reserved.
//

#include <iostream>
#include "GameState.h"
#include "Globals.h"

void GameState::ResetInputs() {
    // Mouse
    this->mouseClicked = false;

    // Gamepad
    this->aGamePad = false;

    // Keyboard
    this->w = false;
	this->a = false;
	this->s = false;
	this->d = false;
	this->up = false;
	this->left = false;
	this->down = false;
	this->right = false;
	this->enter = false;
}

MenuState GameState::GetState() {
    return state;
}

void GameState::SetState(MenuState state) {
    this->state = state;
}

GameState::GameState() {
    this->state = MainMenu;

	for (int y = 0; y < Globals::TILE_ROWS; y++) {
        for (int x = 0; x < Globals::TILE_ROWS; x++) {
            Tile* t = new Tile(x, y);
            tileGrid[x + (y * Globals::TILE_ROWS)] = *t;
            delete t;
        }
    }
}

void GameState::PlayClick() {
    Mix_PlayChannel(-1, clickSound, 0);
}
void GameState::PlayBiscuit() {
    Mix_PlayChannel(-1, biscuitSound, 0);
}
void GameState::PlayPill() {
    Mix_PlayChannel(-1, pillSound, 0);
}
void GameState::PlayHit() {
    Mix_PlayChannel(-1, hitSound, 0);
}
void GameState::PlayGhostDeath() {
    Mix_PlayChannel(-1, ghostDeathSound, 0);
}
