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

using namespace std;
using namespace chrono;

// Methods
void InitialiseSprites();
void ProcessInput(bool&);
void Update(double&);
void Render();
Vector2 NewVector(int x, int y) {
	Vector2 v(x, y); return v;
}
SDL_Rect NewRect(int x, int y, int w, int h) {
	SDL_Rect r = { x, y, w, h }; return r;
}
high_resolution_clock::time_point NowTime() {
	return chrono::high_resolution_clock::now();
}
double TimeSinceLastFrame(high_resolution_clock::time_point frameTime) {
	return (duration_cast<microseconds>(NowTime() - frameTime).count()) / 1000000;
}

// Global Variables
SDL_Window *window;
SDL_Renderer *renderer;
SDL_Rect gameRect = NewRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
SDL_Rect scoreRect = NewRect(0, SCREEN_WIDTH, SCREEN_WIDTH, SCORE_REGION);

GameState gameState;

int main()
{
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
	for (int i = 0; i < N_TILES * N_TILES; i++) {
		Sprite *s = new Sprite(tileTexture, NewRect(gameState.tileGrid[i].GetWidth() * TILE_SIZE, gameState.tileGrid[i].GetHeight() * TILE_SIZE, TILE_SIZE, TILE_SIZE), NewVector(TILE_SIZE / 2, TILE_SIZE / 2));
		gameState.tileGrid[i].SetSprite(*s);
		delete s;
	}

	// Player Textures
	Sprite *p = new Sprite(playerTexture, NewRect(0, 0, -1, -1), NewVector(0, 0));
	gameState.playerSprite = *p;
	delete p;
}

void ProcessInput(bool &running) {
	SDL_Event event;
    while (SDL_PollEvent(&event)) {
		//SDL_Keycode key = event.key.keysym.sym;

		switch (event.type) {
			case SDL_KEYDOWN:
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

	// Play Area Background
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderFillRect(renderer, &gameRect);

	// Score Area Background
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
	SDL_RenderFillRect(renderer, &scoreRect);

	// Tile Sprites
	for (int y = 0; y < N_TILES; y++) {
		for (int x = 0; x < N_TILES; x++) {
			Sprite s = gameState.tileGrid[y + (x * N_TILES)].GetSprite();
			s.Render(renderer);
		}
	}

	// Player Sprite
	gameState.playerSprite.Render(renderer);

	// Background Color
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	// Finalise Render
	SDL_RenderPresent(renderer);
}
