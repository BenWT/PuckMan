//
// Copyright (c) 2016 by Ben Townshend. All Rights Reserved.
//

#include "headers/Globals.h"

#include <iostream>
#include <chrono>
#include <vector>
#include <iterator>
#include "SDL.h"
#include "headers/GameState.h"
#include "headers/Tile.h"
#include "headers/Vector2.h"
#include "headers/Sprite.h"
#include "headers/Player.h"
#include "headers/FontSprite.h"

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
SDL_Window* window;
SDL_Renderer* renderer;
GameState gameState;

int main(int argc, char *argv[]) {
	// Initialise SDL and log failure
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "SDL failed to initialise. \n");
		return 1;
	} else {
		SDL_Log("SDL initialised successfully. \n");
	}

	SDL_CreateWindowAndRenderer(Globals::ACTUAL_SCREEN_WIDTH, Globals::ACTUAL_SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE, &window, &renderer);

	// Log window failure
	if (window == NULL || renderer == NULL) {
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "SDL failed to create a window. \n");
		return 1;
	} else {
		SDL_RenderSetLogicalSize(renderer, Globals::SCREEN_WIDTH, Globals::SCREEN_HEIGHT);
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
	SDL_Surface* biscuitSurface = SDL_LoadBMP("assets/biscuit.bmp");
	// SDL_Surface* fontSurface = SDL_LoadBMP("assets/fonts.bmp");
	// SDL_Surface* fontSelectedSurface = SDL_LoadBMP("assets/fonts-selected.bmp");

	// Textures
	SDL_Texture* playerTexture = SDL_CreateTextureFromSurface(renderer, playerSurface);
	SDL_Texture* tileTexture = SDL_CreateTextureFromSurface(renderer, tileSurface);
	gameState.biscuitTexture = SDL_CreateTextureFromSurface(renderer, biscuitSurface);
	// SDL_Texture* fontTexture = SDL_CreateTextureFromSurface(renderer, fontSurface);
	// SDL_Texture* fontSelectedSurface = SDL_CreateTextureFromSurface(renderer, fontSelectedSurface);

	// Tile Textures
	for (int i = 0; i < Globals::TILE_COUNT; i++) {
		Tile t = gameState.tileGrid[i];
		Sprite *s = new Sprite(
			tileTexture,
			NewRect((int)(t.GetTextureX()), (int)(t.GetTextureY()), Globals::TILE_SIZE, Globals::TILE_SIZE),
			new Vector2((int)(t.GetPositionX()), (int)(t.GetPositionY()))
		);
		gameState.tileGrid[i].SetSprite(*s);
		delete s;
	}

	// Main Menu
	// TODO Create Main Menu objects.

	// Player One Textures
	Player *p1 = new Player(playerTexture, NewRect(0, 0, -1, -1), new Vector2(Globals::TILE_SIZE, Globals::TILE_SIZE));
	p1->tile = Globals::PLAYER_START_X + (Globals::PLAYER_START_Y * Globals::TILE_ROWS);
	p1->SetPositionFromTile(gameState);
	p1->moveDirection = Left;
	gameState.playerSprite = *p1;
	delete p1;

	// Player Two Texture
	Player *p2 = new Player(playerTexture, NewRect(0, 0, -1, -1), new Vector2(Globals::TILE_SIZE, Globals::TILE_SIZE));
	p2->tile = Globals::PLAYER_START_X + (Globals::PLAYER_START_Y * Globals::TILE_ROWS);
	p2->SetPositionFromTile(gameState);
	p2->moveDirection = Right;
	gameState.playerTwoSprite = *p2;
	delete p2;
}

void ProcessInput(bool &running) {
	SDL_Event event;
    while (SDL_PollEvent(&event)) {
		SDL_Keycode key = event.key.keysym.sym;

		switch (event.type) {
			case SDL_MOUSEMOTION:
				gameState.mouseX = event.motion.x;
				gameState.mouseY = event.motion.y;
				break;

			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_LEFT) {
					cout << "Left Click" << endl;
				}
				break;

			case SDL_KEYDOWN:
				if (key == SDLK_w) {
					if (gameState.GetState() == MainMenu) {
						// Menu Move
					} else if (gameState.GetState() == OnePlayer) {
						if (gameState.playerSprite.CanMove(gameState, Up)) gameState.playerSprite.moveDirection = Up;
					}
				}
				if (key == SDLK_s) {
					if (gameState.GetState() == MainMenu) {
						// Menu Move
					} else if (gameState.GetState() == OnePlayer) {
						if (gameState.playerSprite.CanMove(gameState, Down)) gameState.playerSprite.moveDirection = Down;
					}
				}
				if (key == SDLK_a) {
					if (gameState.GetState() == MainMenu) {
						// Menu Move
					} else if (gameState.GetState() == OnePlayer) {
						if (gameState.playerSprite.CanMove(gameState, Left)) gameState.playerSprite.moveDirection = Left;
					}
				}
				if (key == SDLK_d) {
					if (gameState.GetState() == MainMenu) {
						// Menu Move
					} else if (gameState.GetState() == OnePlayer) {
						if (gameState.playerSprite.CanMove(gameState, Right)) gameState.playerSprite.moveDirection = Right;
					}
				}

				if (key == SDLK_UP) {
					if (gameState.GetState() == OnePlayer) {
						if (gameState.playerSprite.CanMove(gameState, Up)) gameState.playerSprite.moveDirection = Up;
					} else if (gameState.GetState() == TwoPlayer) {
						if (gameState.playerTwoSprite.CanMove(gameState, Up)) gameState.playerTwoSprite.moveDirection = Up;
					}
				}
				if (key == SDLK_DOWN) {
					if (gameState.GetState() == OnePlayer) {
						if (gameState.playerSprite.CanMove(gameState, Down)) gameState.playerSprite.moveDirection = Down;
					} else if (gameState.GetState() == TwoPlayer) {
						if (gameState.playerTwoSprite.CanMove(gameState, Down)) gameState.playerTwoSprite.moveDirection = Down;
					}
				}
				if (key == SDLK_LEFT) {
					if (gameState.GetState() == OnePlayer) {
						if (gameState.playerSprite.CanMove(gameState, Left)) gameState.playerSprite.moveDirection = Left;
					} else if (gameState.GetState() == TwoPlayer) {
						if (gameState.playerTwoSprite.CanMove(gameState, Left)) gameState.playerTwoSprite.moveDirection = Left;
					}
				}
				if (key == SDLK_RIGHT) {
					if (gameState.GetState() == OnePlayer) {
						if (gameState.playerSprite.CanMove(gameState, Right)) gameState.playerSprite.moveDirection = Right;
					} else if (gameState.GetState() == TwoPlayer) {
						if (gameState.playerTwoSprite.CanMove(gameState, Right)) gameState.playerTwoSprite.moveDirection = Right;
					}
				}

				if (key == SDLK_ESCAPE) {
					SDL_Log("Program quit.");
		 		    running = false;
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

void Update(double &deltaTime) {
	if (gameState.GetState() == OnePlayer) {
		gameState.playerSprite.DoMove(gameState, deltaTime * Globals::PLAYER_SPEED);
	} else if (gameState.GetState() == TwoPlayer) {
		gameState.playerSprite.DoMove(gameState, deltaTime * Globals::PLAYER_SPEED);
		gameState.playerTwoSprite.DoMove(gameState, deltaTime * Globals::PLAYER_SPEED);
	}
}

void Render() {
	SDL_RenderClear(renderer); // Clear Previous Render

	if (gameState.GetState() == MainMenu) {
		vector<FontSprite>::iterator fs;

		for (fs = gameState.mainMenuText.begin(); fs < gameState.mainMenuText.end(); fs++) {
			fs->Render(renderer);
		}
	} else if (gameState.GetState() == OnePlayer || gameState.GetState() == TwoPlayer) {

		// Tile Sprites
		for (int i = 0; i < Globals::TILE_COUNT; i++) {
			Tile t = gameState.tileGrid[i];
			Sprite s = t.GetSprite();

			s.Render(renderer);

			if (t.CheckBiscuit()) {
				int posX = t.GetPositionX() + 40;
				int posY = t.GetPositionY() + 40;

				SDL_Rect biscuitRect = { posX, posY, 20, 20 };
				SDL_RenderCopy(renderer, gameState.biscuitTexture, NULL, &biscuitRect);
			}
		}

		gameState.playerSprite.Render(renderer); // Player Sprite
		if (gameState.GetState() == TwoPlayer) gameState.playerTwoSprite.Render(renderer); // Second Player Sprite
	}

	// Finalise Render
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderPresent(renderer);
}
