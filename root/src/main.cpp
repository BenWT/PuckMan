//
// Copyright (c) 2016 by Ben Townshend. All Rights Reserved.
//

#include "headers/Globals.h"

#include <iostream>
#include <chrono>
#include "SDL.h"
#include "headers/GameState.h"
#include "headers/Tile.h"
#include "headers/Vector2.h"
#include "headers/Sprite.h"
#include "headers/Player.h"

using namespace std;
using namespace chrono;

// Methods
void InitialiseSprites();
void ProcessInput(bool&);
void Update(double&);
void Render();
SDL_Rect NewRect(int x, int y, int w, int h) {
	SDL_Rect r = { x, y, w, h }; return r;
}
high_resolution_clock::time_point NowTime() {
	return chrono::high_resolution_clock::now();
}
double TimeSinceLastFrame(high_resolution_clock::time_point frameTime) {
	return (duration_cast<microseconds>(NowTime() - frameTime).count()) / 1000000.0;
}

// Global Variables
SDL_Window *window;
SDL_Renderer *renderer;
SDL_Rect gameRect = NewRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
SDL_Rect scoreRect = NewRect(0, SCREEN_WIDTH, SCREEN_WIDTH, SCORE_REGION);

GameState* gameState;

int main(int argc, char *argv[]) {
	// Initialise SDL and log failure
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "SDL failed to initialise. \n");
		return 1;
	} else {
		SDL_Log("SDL initialised successfully. \n");
	}

	SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE, &window, &renderer);

	// Log window failure
	if (window == NULL || renderer == NULL) {
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "SDL failed to create a window. \n");
		return 1;
	} else {
		SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	}

	InitialiseSprites();

	// Game Loop
	bool running = true;
	high_resolution_clock::time_point frameTime = NowTime();
	double deltaTime = 0;

	while (running) {
		deltaTime =  TimeSinceLastFrame(frameTime);
		frameTime = NowTime();

		ProcessInput(running);
		Update(deltaTime);
		Render();
	}

	return 0;
}

void InitialiseSprites() {
	// Surfaces
	SDL_Surface* playerSurface = SDL_LoadBMP("assets/character.bmp");
	SDL_Surface* tileSurface = SDL_LoadBMP("assets/tiles.bmp");

	// Textures
	SDL_Texture* playerTexture = SDL_CreateTextureFromSurface(renderer, playerSurface);
	SDL_Texture* tileTexture = SDL_CreateTextureFromSurface(renderer, tileSurface);

	// Tile Textures
	for (int i = 0; i < TILE_COUNT; i++) {
		Tile t = gameState->tileGrid[i];
		Sprite *s = new Sprite(
			tileTexture,
			NewRect(t.GetWidth() * TILE_SIZE, t.GetHeight() * TILE_SIZE, TILE_SIZE, TILE_SIZE),
			new Vector2(t.GetX() * TILE_SIZE, t.GetY() * TILE_SIZE)
		);
		gameState->tileGrid[i].SetSprite(*s);
		delete s;
	}

	// Player Textures
	Player *p = new Player(playerTexture, NewRect(0, 0, -1, -1), new Vector2(TILE_SIZE, TILE_SIZE));
	p->tile = PLAYER_START_X + (PLAYER_START_Y * TILE_ROWS);
	gameState->playerSprite = *p;
	delete p;
}

void ProcessInput(bool &running) {
	SDL_Event event;
    while (SDL_PollEvent(&event)) {
		SDL_Keycode key = event.key.keysym.sym;

		switch (event.type) {
			case SDL_KEYDOWN:
				if (key == SDLK_w || key == SDLK_UP) {
					gameState->playerSprite.MoveUp(gameState);
				}
				if (key == SDLK_s || key == SDLK_DOWN) {
					gameState->playerSprite.MoveDown(gameState);
				}
				if (key == SDLK_a || key == SDLK_LEFT) {
					gameState->playerSprite.MoveLeft(gameState);
				}
				if (key == SDLK_d || key == SDLK_RIGHT) {
					gameState->playerSprite.MoveRight(gameState);
				}
				break;

			case SDL_KEYUP:
				break;

			case SDL_QUIT:
				SDL_Log("Program quit.");
	 		    running = false;
				break;
		}
	}
}

void Update(double &deltaTime) {  }

void Render() {
	// Clear Previous Render
	SDL_RenderClear(renderer);

	if (gameState->GetState() == Game) {
		// Tile Sprites
		for (int i = 0; i < TILE_COUNT; i++) {
			Sprite s = gameState->tileGrid[i].GetSprite();
			s.Render(renderer);
		}

		// Player Sprite
		gameState->playerSprite.Render(renderer);
	}

	// Finalise Render
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderPresent(renderer);
}
