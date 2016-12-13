//
// Copyright (c) 2016 by Ben Townshend. All Rights Reserved.
//

#include "headers/Globals.h"

#include <iostream>
#include <chrono>
#include "SDL.h"
#include "SDL_image.h"
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
void Update(double&, bool&);
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
	}
	SDL_Log("SDL initialised successfully. \n");

	// Initialise SDL_image and log failure
	int imgFlags = IMG_INIT_PNG;
	if( !( IMG_Init( imgFlags ) & imgFlags ) ) {
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "SDL_image failed to initialise. \n");
		return 1;
	}
	SDL_Log("SDL_image initialised successfully. \n");

	SDL_CreateWindowAndRenderer(Globals::ACTUAL_SCREEN_WIDTH, Globals::ACTUAL_SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE, &window, &renderer);

	// Log window failure
	if (window == NULL || renderer == NULL) {
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "SDL failed to create a window. \n");
		return 1;
	}
	SDL_RenderSetLogicalSize(renderer, Globals::SCREEN_WIDTH, Globals::SCREEN_HEIGHT);

	InitialiseSprites();

	// Game Loop
	bool running = true;
	high_resolution_clock::time_point frameTime = NowTime();
	double deltaTime = 0;

	while (running) {
		deltaTime =  TimeSinceLastFrame(frameTime);
		frameTime = NowTime();

		ProcessInput(running);
		Update(deltaTime, running);
		Render();
	}

	return 0;
}

void InitialiseSprites() {
	// Surfaces
	SDL_Surface* playerSurface = SDL_LoadBMP("assets/character.bmp");
	SDL_Surface* tileSurface = IMG_Load("assets/tiles.png");
	SDL_Surface* biscuitSurface = SDL_LoadBMP("assets/biscuit.bmp");
	SDL_Surface* fontSurface = IMG_Load("assets/fonts.png");
	SDL_Surface* fontSelectedSurface = IMG_Load("assets/fontsSelected.png");

	// Textures
	SDL_Texture* playerTexture = SDL_CreateTextureFromSurface(renderer, playerSurface);
	SDL_Texture* tileTexture = SDL_CreateTextureFromSurface(renderer, tileSurface);
	gameState.biscuitTexture = SDL_CreateTextureFromSurface(renderer, biscuitSurface);
	SDL_Texture* fontTexture = SDL_CreateTextureFromSurface(renderer, fontSurface);
	SDL_Texture* fontSelectedTexture = SDL_CreateTextureFromSurface(renderer, fontSelectedSurface);

	// Tile Textures
	for (int i = 0; i < Globals::TILE_COUNT; i++) {
		Tile t = gameState.tileGrid[i];
		Sprite* s = new Sprite(
			tileTexture,
			NewRect((int)(t.GetTextureX()), (int)(t.GetTextureY()), Globals::TILE_SIZE, Globals::TILE_SIZE),
			new Vector2((int)(t.GetPositionX()), (int)(t.GetPositionY()))
		);
		gameState.tileGrid[i].SetSprite(*s);
		delete s;
	}

	// Main Menu
	int mainMenuScale = 15;
	FontSprite* onePlayer = new FontSprite("One Player", fontTexture, fontSelectedTexture, 0, 0, mainMenuScale, true, true);
	FontSprite* twoPlayer = new FontSprite("Two Player", fontTexture, fontSelectedTexture, 0, mainMenuScale * Globals::FONT_HEIGHT * 1, mainMenuScale, true, true);
	FontSprite* options = new FontSprite("Options", fontTexture, fontSelectedTexture, 0, mainMenuScale * Globals::FONT_HEIGHT * 2, mainMenuScale, true, true);
	FontSprite* quit = new FontSprite("Quit", fontTexture, fontSelectedTexture, 0, mainMenuScale * Globals::FONT_HEIGHT * 3, mainMenuScale, true, true);
	onePlayer->CentreHorizontal();
	twoPlayer->CentreHorizontal();
	options->CentreHorizontal();
	quit->CentreHorizontal();
	gameState.mainMenuText[0] = *onePlayer;
	gameState.mainMenuText[1] = *twoPlayer;
	gameState.mainMenuText[2] = *options;
	gameState.mainMenuText[3] = *quit;
	delete onePlayer;
	delete twoPlayer;
	delete options;
	delete quit;

	// Player One Textures
	Player* p1 = new Player(playerTexture, NewRect(0, 0, -1, -1), new Vector2(Globals::TILE_SIZE, Globals::TILE_SIZE));
	p1->tile = Globals::PLAYER_START_X + (Globals::PLAYER_START_Y * Globals::TILE_ROWS);
	p1->SetPositionFromTile(gameState);
	p1->moveDirection = Left;
	gameState.playerSprite = *p1;
	delete p1;

	// Player Two Texture
	Player* p2 = new Player(playerTexture, NewRect(0, 0, -1, -1), new Vector2(Globals::TILE_SIZE, Globals::TILE_SIZE));
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

				if (gameState.GetState() == MainMenu) {
					for (int i = 0; i < Globals::MAIN_MENU_ITEMS; i++) {
						if (gameState.mainMenuText[i].canSelect) {
							gameState.mainMenuText[i].selected = gameState.mainMenuText[i].CheckBounds(gameState.mouseX, gameState.mouseY);
						}
					}
				}
				break;

			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_LEFT) {
					for (int i = 0; i < Globals::MAIN_MENU_ITEMS; i++) {
						if (gameState.mainMenuText[i].canClick) {
							if (gameState.mainMenuText[i].CheckBounds(gameState.mouseX, gameState.mouseY)) {
								gameState.mainMenuText[i].clicked = true;
							}
						}
					}
				}
				break;

			case SDL_KEYDOWN:
				if (key == SDLK_w) {
					if (gameState.GetState() == MainMenu) {
						// Menu Move
					} else if (gameState.GetState() == OnePlayer || gameState.GetState() == TwoPlayer) {
						if (gameState.playerSprite.CanMove(gameState, Up)) gameState.playerSprite.moveDirection = Up;
					}
				}
				if (key == SDLK_s) {
					if (gameState.GetState() == MainMenu) {
						// Menu Move
					} else if (gameState.GetState() == OnePlayer || gameState.GetState() == TwoPlayer) {
						if (gameState.playerSprite.CanMove(gameState, Down)) gameState.playerSprite.moveDirection = Down;
					}
				}
				if (key == SDLK_a) {
					if (gameState.GetState() == MainMenu) {
						// Menu Move
					} else if (gameState.GetState() == OnePlayer || gameState.GetState() == TwoPlayer) {
						if (gameState.playerSprite.CanMove(gameState, Left)) gameState.playerSprite.moveDirection = Left;
					}
				}
				if (key == SDLK_d) {
					if (gameState.GetState() == MainMenu) {
						// Menu Move
					} else if (gameState.GetState() == OnePlayer || gameState.GetState() == TwoPlayer) {
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

void Update(double &deltaTime, bool &running) {
	if (gameState.GetState() == MainMenu) {
		// TODO Find a better way to do this...
		if (gameState.mainMenuText[0].clicked) { // One Player Button
			gameState.mainMenuText[0].DoClick();
			gameState.SetState(OnePlayer);
		}
		else if (gameState.mainMenuText[1].clicked) { // Two Player Button
			gameState.mainMenuText[1].DoClick();
			gameState.SetState(TwoPlayer);
		}
		else if (gameState.mainMenuText[2].clicked) { // Options Button
			gameState.mainMenuText[2].DoClick();
		}
		else if (gameState.mainMenuText[3].clicked) { // Quit Button
			gameState.mainMenuText[3].DoClick();
			running = false;
		}
	}
	// TODO Uncomment player score when text is loading
	if (gameState.GetState() == OnePlayer) {
		gameState.playerSprite.DoMove(gameState, deltaTime * Globals::PLAYER_SPEED);
		// gameState.playerScoreText.text = "Score: " + gameState.playerSprite.score;
	} else if (gameState.GetState() == TwoPlayer) {
		gameState.playerSprite.DoMove(gameState, deltaTime * Globals::PLAYER_SPEED);
		gameState.playerTwoSprite.DoMove(gameState, deltaTime * Globals::PLAYER_SPEED);
		// gameState.playerScoreText.text = "Score: " + gameState.playerSprite.score;
		// gameState.playerTwoScoreText.text = "Score: " + gameState.playerTwoSprite.score;
	}
}

void Render() {
	SDL_RenderClear(renderer); // Clear Previous Render

	if (gameState.GetState() == MainMenu) {
		for (int i = 0; i < Globals::MAIN_MENU_ITEMS; i++) {
			gameState.mainMenuText[i].Render(renderer);
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
