//
// Copyright (c) 2016 by Ben Townshend. All Rights Reserved.
//

#include "headers/Globals.h"

#include <iostream>
#include <chrono>
#include <cmath>
#include "SDL.h"
#include "SDL_gamecontroller.h"
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
SDL_Joystick* joystick;
GameState gameState;

int main(int argc, char *argv[]) {
	// Initialise SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) != 0) {
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "SDL failed to initialise. \n");
		return 1;
	}
	SDL_Log("SDL initialised successfully. \n");

	// Initialise SDL_image
	int imgFlags = IMG_INIT_PNG;
	if( !( IMG_Init( imgFlags ) & imgFlags ) ) {
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "SDL_image failed to initialise. \n");
		return 1;
	}
	SDL_Log("SDL_image initialised successfully. \n");

	// Create Window
	SDL_CreateWindowAndRenderer(Globals::ACTUAL_SCREEN_WIDTH, Globals::ACTUAL_SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE, &window, &renderer);
	if (window == NULL || renderer == NULL) {
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "SDL failed to create a window. \n");
		return 1;
	}
	SDL_RenderSetLogicalSize(renderer, Globals::SCREEN_WIDTH, Globals::SCREEN_HEIGHT);

	// Create Gamepad
	for (int i = 0; i < SDL_NumJoysticks(); i++) {
		joystick = SDL_JoystickOpen(i);
	   	if (joystick) {
			SDL_Log("Joystick initialised successfully. \n");
		} else {
			SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "SDL failed to load joystick. \n");
		}
	}

	// Create Sprites
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

	// Clean up on close
	SDL_JoystickClose(joystick);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
    joystick = NULL;
	renderer = NULL;
	window = NULL;
	IMG_Quit();
	SDL_Quit();

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
	SDL_Texture* font = SDL_CreateTextureFromSurface(renderer, fontSurface);
	SDL_Texture* fontS = SDL_CreateTextureFromSurface(renderer, fontSelectedSurface);

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
	FontSprite* title = new FontSprite("PuckMan!", font, fontS, 0, 0, mainMenuScale * 2, false, false);
	FontSprite* onePlayer = new FontSprite("One Player", font, fontS, 0, mainMenuScale * Globals::FONT_HEIGHT * 2, mainMenuScale, true, true);
	FontSprite* twoPlayer = new FontSprite("Two Player", font, fontS, 0, mainMenuScale * Globals::FONT_HEIGHT * 3, mainMenuScale, true, true);
	FontSprite* options = new FontSprite("Options", font, fontS, 0, mainMenuScale * Globals::FONT_HEIGHT * 4, mainMenuScale, true, true);
	FontSprite* quit = new FontSprite("Quit", font, fontS, 0, mainMenuScale * Globals::FONT_HEIGHT * 5, mainMenuScale, true, true);
	title->CentreHorizontal();
	onePlayer->CentreHorizontal();
	twoPlayer->CentreHorizontal();
	options->CentreHorizontal();
	quit->CentreHorizontal();
	gameState.mainMenuText[0] = *title;
	gameState.mainMenuText[1] = *onePlayer;
	gameState.mainMenuText[2] = *twoPlayer;
	gameState.mainMenuText[3] = *options;
	gameState.mainMenuText[4] = *quit;
	delete title;
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
	gameState.ResetInputs();

	SDL_Event event;
	while (SDL_PollEvent(&event)) {
	    switch (event.type) {
			case SDL_MOUSEMOTION:
				gameState.mouseX = event.motion.x;
				gameState.mouseY = event.motion.y;
				break;

			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_LEFT) {
					gameState.mouseClicked = true;
				}
				break;

			case SDL_JOYHATMOTION:
				if (event.jhat.which == 0) {
					switch (event.jhat.value) {
						case SDL_HAT_UP: gameState.w = true; break;
						case SDL_HAT_LEFT: gameState.a = true; break;
						case SDL_HAT_DOWN: gameState.s = true; break;
						case SDL_HAT_RIGHT: gameState.d = true; break;
					}
				}
				break;

			case SDL_JOYBUTTONDOWN:
				// TODO Implement Gamepad buttons
				break;

			case SDL_JOYAXISMOTION:
				// TODO Implement joysticks
				if (event.jaxis.which == 0) {
					switch (event.jaxis.axis) {
						case 0: gameState.leftJoystickX = event.jaxis.value; break;
						case 1: gameState.leftJoystickY = event.jaxis.value; break;
						case 2: break; // Triggers

						#ifdef _WIN32 // Windows reverses X and Y axis of right joystick
							case 3: gameState.rightJoystickY = event.jaxis.value; break;
							case 4: gameState.rightJoystickX = event.jaxis.value; break;
						#else
							case 3: gameState.rightJoystickX = event.jaxis.value; break;
							case 4: gameState.rightJoystickY = event.jaxis.value; break;
						#endif
					}
				}
				break;

			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
					case SDLK_w: gameState.w = true; break;
					case SDLK_a: gameState.a = true; break;
					case SDLK_s: gameState.s = true; break;
					case SDLK_d: gameState.d = true; break;
					case SDLK_UP: gameState.up = true; break;
					case SDLK_LEFT: gameState.left = true; break;
					case SDLK_DOWN: gameState.down = true; break;
					case SDLK_RIGHT: gameState.right = true; break;
					case SDLK_KP_ENTER: gameState.enter = true; break;
					case SDLK_RETURN: gameState.enter = true; break;
					case SDLK_ESCAPE: SDL_Log("Program quit."); running = false; break;
				}
				break;

			case SDL_QUIT:
				SDL_Log("Program quit.");
	 		    running = false;
				break;
		}
	}


}

void Update(double &deltaTime, bool &running) {
	double speed = deltaTime * Globals::PLAYER_SPEED;
	bool p1Up = false, p1Left = false, p1Down = false, p1Right = false;
	bool p2Up = false, p2Left = false, p2Down = false, p2Right = false;

	if (gameState.GetState() == MainMenu) {
		// TODO Implement Main menu logic
	} else if (gameState.GetState() == OnePlayer) {
		// Player One
		if (gameState.w || gameState.up || gameState.leftJoystickY < -Globals::JOYSTICK_DEAD_ZONE) p1Up = true;
		else if (gameState.a || gameState.left || gameState.leftJoystickX < -Globals::JOYSTICK_DEAD_ZONE) p1Left = true;
		else if (gameState.s || gameState.down || gameState.leftJoystickY > Globals::JOYSTICK_DEAD_ZONE) p1Down = true;
		else if (gameState.d || gameState.right || gameState.leftJoystickX > Globals::JOYSTICK_DEAD_ZONE) p1Right = true;
	} else if (gameState.GetState() == TwoPlayer) {
		// Player One
		if (gameState.w || gameState.leftJoystickY < -Globals::JOYSTICK_DEAD_ZONE) p1Up = true;
		else if (gameState.a || gameState.leftJoystickX < -Globals::JOYSTICK_DEAD_ZONE) p1Left = true;
		else if (gameState.s || gameState.leftJoystickY > Globals::JOYSTICK_DEAD_ZONE) p1Down = true;
		else if (gameState.d || gameState.leftJoystickX > Globals::JOYSTICK_DEAD_ZONE) p1Right = true;

		// Player Two
		if (gameState.up || gameState.rightJoystickY < -Globals::JOYSTICK_DEAD_ZONE) p2Up = true;
		else if (gameState.left || gameState.rightJoystickX < -Globals::JOYSTICK_DEAD_ZONE) p2Left = true;
		else if (gameState.down || gameState.rightJoystickY > Globals::JOYSTICK_DEAD_ZONE) p2Down = true;
		else if (gameState.right || gameState.rightJoystickX > Globals::JOYSTICK_DEAD_ZONE) p2Right = true;
	}

	if (gameState.GetState() == OnePlayer || gameState.GetState() == TwoPlayer) {
		if (p1Up && gameState.playerSprite.CanMove(gameState, Up)) gameState.playerSprite.moveDirection = Up;
		if (p1Left && gameState.playerSprite.CanMove(gameState, Left)) gameState.playerSprite.moveDirection = Left;
		if (p1Down && gameState.playerSprite.CanMove(gameState, Down)) gameState.playerSprite.moveDirection = Down;
		if (p1Right && gameState.playerSprite.CanMove(gameState, Right)) gameState.playerSprite.moveDirection = Right;

		if (p2Up && gameState.playerTwoSprite.CanMove(gameState, Up)) gameState.playerTwoSprite.moveDirection = Up;
		if (p2Left && gameState.playerTwoSprite.CanMove(gameState, Left)) gameState.playerTwoSprite.moveDirection = Left;
		if (p2Down && gameState.playerTwoSprite.CanMove(gameState, Down)) gameState.playerTwoSprite.moveDirection = Down;
		if (p2Right && gameState.playerTwoSprite.CanMove(gameState, Right)) gameState.playerTwoSprite.moveDirection = Right;

		gameState.playerSprite.DoMove(gameState, speed);
		if (gameState.GetState() == TwoPlayer) gameState.playerTwoSprite.DoMove(gameState, speed);
	}



	/* if (gameState.GetState() == MainMenu) {
		if (gameState.GetState() == MainMenu) {
			for (int i = 0; i < Globals::MAIN_MENU_ITEMS; i++) {
				if (gameState.mainMenuText[i].canSelect) {
					gameState.mainMenuText[i].selected = gameState.mainMenuText[i].CheckBounds(gameState.mouseX, gameState.mouseY);
				}
			}
		}

		// TODO Implement a better way to do this, with std::function
		if (gameState.mainMenuText[1].clicked) { // One Player Button
			gameState.mainMenuText[1].DoClick();
			gameState.SetState(OnePlayer);
		}
		else if (gameState.mainMenuText[2].clicked) { // Two Player Button
			gameState.mainMenuText[2].DoClick();
			gameState.SetState(TwoPlayer);
		}
		else if (gameState.mainMenuText[3].clicked) { // Options Button
			gameState.mainMenuText[3].DoClick();
		}
		else if (gameState.mainMenuText[4].clicked) { // Quit Button
			gameState.mainMenuText[4].DoClick();
			running = false;
		}
	}
	// TODO Show Player Score
	double speed = deltaTime * Globals::PLAYER_SPEED;
	if (gameState.GetState() == OnePlayer) {
		gameState.playerSprite.DoMove(gameState, speed); // TODO Keyboard affected by joystick movement
		// gameState.playerScoreText.text = "Score: " + gameState.playerSprite.score;
	} else if (gameState.GetState() == TwoPlayer) {
		gameState.playerSprite.DoMove(gameState, speed);
		gameState.playerTwoSprite.DoMove(gameState, speed);
		// gameState.playerScoreText.text = "Score: " + gameState.playerSprite.score;
		// gameState.playerTwoScoreText.text = "Score: " + gameState.playerTwoSprite.score;
	} */
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
