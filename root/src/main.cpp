//
// Copyright (c) 2016 by Ben Townshend. All Rights Reserved.
//

// MSVCP140D.dll
// VCRUNTIME140D.dll
// ucrtbased.dll

#include "headers/Globals.h"

#include <iostream>
#include <chrono>
#include <string>
#include <cmath>
#include "SDL.h"
#include "SDL_gamecontroller.h"
#include "SDL_image.h"
#include "headers/GameState.h"
#include "headers/Tile.h"
#include "headers/Vector2.h"
#include "headers/Sprite.h"
#include "headers/Player.h"
#include "headers/Enemy.h"
#include "headers/FontSprite.h"

using namespace std;
using namespace chrono;

// Methods
void InitialiseSprites();
void ProcessInput();
void Update(double&);
void Render();
void Reset();
SDL_Rect NewRect(int x, int y, int w, int h) {
	SDL_Rect r = { x, y, w, h }; return r;
}
high_resolution_clock::time_point NowTime() {
	return chrono::high_resolution_clock::now();
}
double TimeSinceLastFrame(high_resolution_clock::time_point frameTime) {
	return (duration_cast<microseconds>(NowTime() - frameTime).count()) / 1000000.0;
}
string basePath;
string GetPathFromFullPath(const string& str) {
	size_t found = str.find_last_of("/\\");
	if (found == numeric_limits<size_t>::max()) return "";
	else return (str.substr(0, found) + "\\");
}
string GetfilenameFromFullPath(const string& str) {
	size_t found = str.find_last_of("/\\");
	if (found == numeric_limits<size_t>::max()) return str;
	else return (str.substr(found + 1));
}
string AddBase(string path) {
	return basePath + path;
}

// Global Variables
SDL_Window* window;
SDL_Renderer* renderer;
SDL_Joystick* joystick;
GameState gameState;
bool running;

// Menu Callback Functions
void onePlayerCallback() {
	Reset();
	gameState.SetState(OnePlayer);
}
void twoPlayerCallback() {
	Reset();
	gameState.SetState(TwoPlayer);
}
void optionsCallback() {
	cout << "clicked options" << endl;
}
void quitCallback() {
	SDL_Log("Program quit.");
	running = false;
}
void retryOnePlayerCallback() {
	Reset();
	gameState.SetState(OnePlayer);
}
void retryTwoPlayerCallback() {
	Reset();
	gameState.SetState(TwoPlayer);
}
void backToMenuCallback() {
	gameState.SetState(MainMenu);
}

int main(int argc, char *argv[]) {
	// Get Base Path
	basePath = argv[0];
	basePath = GetPathFromFullPath(basePath).c_str();

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
	running = true;
	high_resolution_clock::time_point frameTime = NowTime();
	double deltaTime = 0;

	while (running) {
		deltaTime =  TimeSinceLastFrame(frameTime);
		frameTime = NowTime();

		ProcessInput();
		Update(deltaTime);
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
	cout << AddBase("123").c_str() << endl;
	SDL_Surface* playerSurface = IMG_Load(AddBase("assets/puckman.png").c_str());
	SDL_Surface* enemySurface = IMG_Load(AddBase("assets/ghost.png").c_str());
	SDL_Surface* tileSurface = IMG_Load(AddBase("assets/tiles.png").c_str());
	SDL_Surface* biscuitSurface = SDL_LoadBMP(AddBase("assets/biscuit.bmp").c_str());
	SDL_Surface* fontSurface = IMG_Load(AddBase("assets/fonts.png").c_str());
	SDL_Surface* fontSelectedSurface = IMG_Load(AddBase("assets/fontsSelected.png").c_str());

	// Textures
	SDL_Texture* playerTexture = SDL_CreateTextureFromSurface(renderer, playerSurface);
	SDL_Texture* enemyTexture = SDL_CreateTextureFromSurface(renderer, enemySurface);
	SDL_Texture* tileTexture = SDL_CreateTextureFromSurface(renderer, tileSurface);
	gameState.biscuitTexture = SDL_CreateTextureFromSurface(renderer, biscuitSurface);
	SDL_Texture* font = SDL_CreateTextureFromSurface(renderer, fontSurface);
	SDL_Texture* fontS = SDL_CreateTextureFromSurface(renderer, fontSelectedSurface);

	// Main Menu
	int mainMenuScale = 15;
	FontSprite* title = new FontSprite("PuckMan!", font, fontS, 0, 0, mainMenuScale * 2, false, false);
	FontSprite* onePlayer = new FontSprite("One Player", font, fontS, 0, mainMenuScale * Globals::FONT_HEIGHT * 2, mainMenuScale, true, true, onePlayerCallback);
	FontSprite* twoPlayer = new FontSprite("Two Player", font, fontS, 0, mainMenuScale * Globals::FONT_HEIGHT * 3, mainMenuScale, true, true, twoPlayerCallback);
	FontSprite* options = new FontSprite("Options", font, fontS, 0, mainMenuScale * Globals::FONT_HEIGHT * 4, mainMenuScale, true, true, optionsCallback);
	FontSprite* quit = new FontSprite("Quit", font, fontS, 0, mainMenuScale * Globals::FONT_HEIGHT * 5, mainMenuScale, true, true, quitCallback);
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

	// One Player Death Menu
	FontSprite* gameOver = new FontSprite("Game Over!", font, fontS, 0, 0, mainMenuScale * 2, false, false);
	FontSprite* retryOnePlayer = new FontSprite("Retry?", font, fontS, 0, Globals::SCREEN_HEIGHT - (2 * Globals::FONT_HEIGHT * mainMenuScale), mainMenuScale, true, true, retryOnePlayerCallback);
	FontSprite* retryTwoPlayer = new FontSprite("Retry?", font, fontS, 0, Globals::SCREEN_HEIGHT - (2 * Globals::FONT_HEIGHT * mainMenuScale), mainMenuScale, true, true, retryTwoPlayerCallback);
	FontSprite* mainMenu = new FontSprite("Back to Menu", font, fontS, 0, Globals::SCREEN_HEIGHT - (Globals::FONT_HEIGHT * mainMenuScale), mainMenuScale, true, true, backToMenuCallback);
	gameOver->CentreHorizontal();
	retryOnePlayer->CentreHorizontal();
	retryTwoPlayer->CentreHorizontal();
	mainMenu->CentreHorizontal();
	gameState.endGameOneText[0] = *gameOver;
	gameState.endGameTwoText[0] = *gameOver;
	gameState.endGameOneText[1] = *retryOnePlayer;
	gameState.endGameTwoText[1] = *retryTwoPlayer;
	gameState.endGameOneText[2] = *mainMenu;
	gameState.endGameTwoText[2] = *mainMenu;
	delete gameOver;

	// Score Text
	FontSprite* playerOneScoreText = new FontSprite("Score: ", font, fontS, 80, Globals::TILE_ROWS * Globals::TILE_SIZE, 10, false, false);
	FontSprite* playerTwoScoreText = new FontSprite("Score: ", font, fontS, (Globals::TILE_ROWS * Globals::TILE_SIZE / 2) + Globals::TILE_SIZE, Globals::TILE_ROWS * Globals::TILE_SIZE, 10, false, false);
	gameState.playerScoreText = *playerOneScoreText;
	gameState.playerTwoScoreText = *playerTwoScoreText;
	delete playerOneScoreText;
	delete playerTwoScoreText;

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

	for (int i = 0; i < 4; i++) {
		Enemy* e = new Enemy(enemyTexture, NewRect(0, 0, -1, -1), new Vector2(0, 0));
		e->tile = Globals::ENEMY_START_X[i] + (Globals::ENEMY_START_Y[i] * Globals::TILE_ROWS);
		e->SetPositionFromTile(gameState);
		e->moveDirection = Left;
		e->canBiscuit = false;
		e->doAnimate = true;
		e->frameTime = 0.15;
		for (int j = 0; j < 2; j++) {
			e->animStates.push_back(NewRect(j * 100,i * 100, 100, 100));
		}
		gameState.enemySprites[i] = *e;
		delete e;
	}

	// Player One Textures
	Player* p1 = new Player(playerTexture, NewRect(0, 0, -1, -1), new Vector2(0, 0));
	p1->tile = Globals::PLAYER_START_X + (Globals::PLAYER_START_Y * Globals::TILE_ROWS);
	p1->SetPositionFromTile(gameState);
	p1->moveDirection = Left;
	p1->doAnimate = true;
	p1->frameTime = 0.025;
	for (int i = 0; i < 14; i++) {
		p1->animStates.push_back(NewRect(i * 100, 0, 100, 100));
	}
	gameState.playerSprite = *p1;
	delete p1;

	// Player Two Texture
	Player* p2 = new Player(playerTexture, NewRect(0, 0, -1, -1), new Vector2(0, 0));
	p2->tile = Globals::PLAYER_START_X + (Globals::PLAYER_START_Y * Globals::TILE_ROWS);
	p2->SetPositionFromTile(gameState);
	p2->moveDirection = Right;
	p2->doAnimate = true;
	p2->frameTime = 0.025;
	for (int i = 0; i < 14; i++) {
		p2->animStates.push_back(NewRect(i * 100, 0, 100, 100));
	}
	gameState.playerTwoSprite = *p2;
	delete p2;
}

void ProcessInput() {
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
				if (event.jbutton.which == 0) {
					if (event.jbutton.button == 0) {
						gameState.aGamePad = true;
					} else if (event.jbutton.button == 1) {
						SDL_Log("Program quit.");
			 		    running = false;
					}
				}
				break;

			case SDL_JOYAXISMOTION:
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
					case SDLK_UP:
						gameState.up = true;
						break;
					case SDLK_LEFT:
						gameState.left = true;
						break;
					case SDLK_DOWN:
						gameState.down = true;
						break;
					case SDLK_RIGHT:
						gameState.right = true;
						break;
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

void Update(double &deltaTime) {
	double p1Speed = deltaTime * Globals::PLAYER_SPEED;
	double p2Speed = deltaTime * Globals::PLAYER_SPEED;
	bool selectedWithMouse = false;
	bool p1Up = false, p1Left = false, p1Down = false, p1Right = false;
	bool p2Up = false, p2Left = false, p2Down = false, p2Right = false;

	if (gameState.GetState() == MainMenu) {
		for (int i = 0; i < Globals::MAIN_MENU_ITEMS; i++) {
			if (gameState.mainMenuText[i].canSelect) {
				gameState.mainMenuText[i].selected = gameState.mainMenuText[i].CheckBounds(gameState.mouseX, gameState.mouseY);

				if (gameState.mainMenuText[i].selected) {
					gameState.mainMenuSelectionIndex = i;
					selectedWithMouse = true;
				}
			}

			if (gameState.mouseClicked) {
				if (gameState.mainMenuText[i].canClick) {
					if (gameState.mainMenuText[i].CheckBounds(gameState.mouseX, gameState.mouseY)) {
						gameState.mainMenuText[i].DoClick();
					}
				}
			}
		}

		if (!selectedWithMouse) {
			bool moveUp = false, moveDown = false;

			if (gameState.leftJoystickY > Globals::JOYSTICK_DEAD_ZONE && gameState.joystickTimer >= gameState.joystickSwapTime) {
				moveDown = true;
				gameState.joystickTimer = 0.0;
			} else if (gameState.leftJoystickY < -Globals::JOYSTICK_DEAD_ZONE && gameState.joystickTimer >= gameState.joystickSwapTime) {
				moveUp = true;
				gameState.joystickTimer = 0.0;
			}

			if (gameState.w || gameState.up) moveUp = true;
			else if (gameState.s || gameState.down) moveDown = true;

			if (moveUp) {
				int newIndex = gameState.mainMenuSelectionIndex - 1;
				if (newIndex >= 0 && gameState.mainMenuText[newIndex].canSelect) gameState.mainMenuSelectionIndex--;
			} else if (moveDown) {
				int newIndex = gameState.mainMenuSelectionIndex + 1;
				if (newIndex < Globals::MAIN_MENU_ITEMS && gameState.mainMenuText[newIndex].canSelect) gameState.mainMenuSelectionIndex++;
			}

			for (int i = 0; i < Globals::MAIN_MENU_ITEMS; i++) {
				if (gameState.mainMenuText[i].canSelect) {
					gameState.mainMenuText[i].selected = (i == gameState.mainMenuSelectionIndex);
				}
			}
		}

		if (gameState.enter || gameState.aGamePad) {
			if (gameState.mainMenuText[gameState.mainMenuSelectionIndex].canClick)
				gameState.mainMenuText[gameState.mainMenuSelectionIndex].DoClick();
		}
	} else if (gameState.GetState() == EndGameOnePlayer) {
		for (int i = 0; i < Globals::END_GAME_ONE_ITEMS; i++) {
			if (gameState.endGameOneText[i].canSelect) {
				gameState.endGameOneText[i].selected = gameState.endGameOneText[i].CheckBounds(gameState.mouseX, gameState.mouseY);

				if (gameState.endGameOneText[i].selected) {
					gameState.endGameOneSelectionIndex = i;
					selectedWithMouse = true;
				}
			}

			if (gameState.mouseClicked) {
				if (gameState.endGameOneText[i].canClick) {
					if (gameState.endGameOneText[i].CheckBounds(gameState.mouseX, gameState.mouseY)) {
						gameState.endGameOneText[i].DoClick();
					}
				}
			}
		}

		if (!selectedWithMouse) {
			bool moveUp = false, moveDown = false;

			if (gameState.leftJoystickY > Globals::JOYSTICK_DEAD_ZONE && gameState.joystickTimer >= gameState.joystickSwapTime) {
				moveDown = true;
				gameState.joystickTimer = 0.0;
			} else if (gameState.leftJoystickY < -Globals::JOYSTICK_DEAD_ZONE && gameState.joystickTimer >= gameState.joystickSwapTime) {
				moveUp = true;
				gameState.joystickTimer = 0.0;
			}

			// Crash in here

			if (gameState.w || gameState.up) moveUp = true;
			else if (gameState.s || gameState.down) moveDown = true;

			if (moveUp) {
				int newIndex = gameState.endGameOneSelectionIndex - 1;
				if (newIndex >= 0 && gameState.endGameOneText[newIndex].canSelect) gameState.endGameOneSelectionIndex--;
			} else if (moveDown) {
				int newIndex = gameState.endGameOneSelectionIndex + 1;
				if (newIndex < Globals::MAIN_MENU_ITEMS && gameState.endGameOneText[newIndex].canSelect) gameState.endGameOneSelectionIndex++;
			}

			for (int i = 0; i < Globals::END_GAME_ONE_ITEMS; i++) {
				if (gameState.endGameOneText[i].canSelect) {
					gameState.endGameOneText[i].selected = (i == gameState.endGameOneSelectionIndex);
				}
			}
		}

		if (gameState.enter || gameState.aGamePad) {
			if (gameState.endGameOneText[gameState.endGameOneSelectionIndex].canClick)
				gameState.endGameOneText[gameState.endGameOneSelectionIndex].DoClick();
		}
	} else if (gameState.GetState() == EndGameTwoPlayer) {
		for (int i = 0; i < Globals::END_GAME_TWO_ITEMS; i++) {
			if (gameState.endGameTwoText[i].canSelect) {
				gameState.endGameTwoText[i].selected = gameState.endGameTwoText[i].CheckBounds(gameState.mouseX, gameState.mouseY);

				if (gameState.endGameTwoText[i].selected) {
					gameState.endGameTwoSelectionIndex = i;
					selectedWithMouse = true;
				}
			}

			if (gameState.mouseClicked) {
				if (gameState.endGameTwoText[i].canClick) {
					if (gameState.endGameTwoText[i].CheckBounds(gameState.mouseX, gameState.mouseY)) {
						gameState.endGameTwoText[i].DoClick();
					}
				}
			}
		}

		if (!selectedWithMouse) {
			bool moveUp = false, moveDown = false;

			if (gameState.leftJoystickY > Globals::JOYSTICK_DEAD_ZONE && gameState.joystickTimer >= gameState.joystickSwapTime) {
				moveDown = true;
				gameState.joystickTimer = 0.0;
			} else if (gameState.leftJoystickY < -Globals::JOYSTICK_DEAD_ZONE && gameState.joystickTimer >= gameState.joystickSwapTime) {
				moveUp = true;
				gameState.joystickTimer = 0.0;
			}

			if (gameState.w || gameState.up) moveUp = true;
			else if (gameState.s || gameState.down) moveDown = true;

			if (moveUp) {
				int newIndex = gameState.endGameTwoSelectionIndex - 1;
				if (newIndex >= 0 && gameState.endGameTwoText[newIndex].canSelect) gameState.endGameTwoSelectionIndex--;
			} else if (moveDown) {
				int newIndex = gameState.endGameTwoSelectionIndex + 1;
				if (newIndex < Globals::MAIN_MENU_ITEMS && gameState.endGameTwoText[newIndex].canSelect) gameState.endGameTwoSelectionIndex++;
			}

			for (int i = 0; i < Globals::END_GAME_TWO_ITEMS; i++) {
				if (gameState.endGameTwoText[i].canSelect) {
					gameState.endGameTwoText[i].selected = (i == gameState.endGameTwoSelectionIndex);
				}
			}
		}

		if (gameState.enter || gameState.aGamePad) {
			if (gameState.endGameTwoText[gameState.endGameTwoSelectionIndex].canClick)
				gameState.endGameTwoText[gameState.endGameTwoSelectionIndex].DoClick();
		}
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
		if (gameState.playerSprite.alive) {
			if (p1Up && gameState.playerSprite.CanMove(gameState, Up)) gameState.playerSprite.moveDirection = Up;
			if (p1Left && gameState.playerSprite.CanMove(gameState, Left)) gameState.playerSprite.moveDirection = Left;
			if (p1Down && gameState.playerSprite.CanMove(gameState, Down)) gameState.playerSprite.moveDirection = Down;
			if (p1Right && gameState.playerSprite.CanMove(gameState, Right)) gameState.playerSprite.moveDirection = Right;
		}

		if (gameState.playerTwoSprite.alive) {
			if (p2Up && gameState.playerTwoSprite.CanMove(gameState, Up)) gameState.playerTwoSprite.moveDirection = Up;
			if (p2Left && gameState.playerTwoSprite.CanMove(gameState, Left)) gameState.playerTwoSprite.moveDirection = Left;
			if (p2Down && gameState.playerTwoSprite.CanMove(gameState, Down)) gameState.playerTwoSprite.moveDirection = Down;
			if (p2Right && gameState.playerTwoSprite.CanMove(gameState, Right)) gameState.playerTwoSprite.moveDirection = Right;
		}

		for (int i = 0; i < 4; i++) {
			if (gameState.playerSprite.tile == gameState.enemySprites[i].tile) {
				gameState.playerSprite.alive = false;
			}
			if (gameState.GetState() == TwoPlayer) {
				if (gameState.playerTwoSprite.tile == gameState.enemySprites[i].tile) {
					gameState.playerTwoSprite.alive = false;
				}
			}
		}

		if (gameState.GetState() == OnePlayer) {
			if (!gameState.playerSprite.alive) {
				gameState.SetState(EndGameOnePlayer);
			}
		} else if (gameState.GetState() == TwoPlayer) {
			if (!gameState.playerSprite.alive && !gameState.playerTwoSprite.alive) {
				gameState.SetState(EndGameTwoPlayer);
			}
		}

		double leftJoyStickValue = 1;
		double rightJoyStickValue = 1;
		if (joystick) {
			leftJoyStickValue = max(abs(gameState.leftJoystickX), abs(gameState.leftJoystickY)) / 32767.0;
			rightJoyStickValue = max(abs(gameState.rightJoystickX), abs(gameState.rightJoystickY)) / 32767.0;
		}

		if (gameState.GetState() == OnePlayer) {
			gameState.playerSprite.DoMove(gameState, p1Speed * max(leftJoyStickValue, rightJoyStickValue), deltaTime);

			gameState.playerScoreText.ChangeText("Score: " + to_string(gameState.playerSprite.score));
		}
		else if (gameState.GetState() == TwoPlayer) {
			gameState.playerSprite.DoMove(gameState, p1Speed * leftJoyStickValue, deltaTime);
			gameState.playerTwoSprite.DoMove(gameState, p2Speed * rightJoyStickValue, deltaTime);

			gameState.playerScoreText.ChangeText("Score: " + to_string(gameState.playerSprite.score));
			gameState.playerTwoScoreText.ChangeText("Score: " + to_string(gameState.playerTwoSprite.score));
		}

		for (int i = 0; i < 4; i++) {
			gameState.enemySprites[i].PathFind(gameState, deltaTime);
		}
	}

	gameState.joystickTimer += deltaTime;
}

void Render() {
	SDL_RenderClear(renderer); // Clear Previous Render

	if (gameState.GetState() == MainMenu) {
		for (int i = 0; i < Globals::MAIN_MENU_ITEMS; i++) {
			gameState.mainMenuText[i].Render(renderer);
		}
	} else if (gameState.GetState() == EndGameOnePlayer) {
		for (int i = 0; i < Globals::END_GAME_ONE_ITEMS; i++) {
			gameState.endGameOneText[i].Render(renderer);
		}
	} else if (gameState.GetState() == EndGameTwoPlayer) {
		for (int i = 0; i < Globals::END_GAME_TWO_ITEMS; i++) {
			gameState.endGameTwoText[i].Render(renderer);
		}
	} else if (gameState.GetState() == OnePlayer || gameState.GetState() == TwoPlayer) {

		// Tile Sprites
		for (int i = 0; i < Globals::TILE_COUNT; i++) {
			gameState.tileGrid[i].GetSprite().Render(renderer);

			if (gameState.tileGrid[i].CheckBiscuit()) {
				int posX = (int)(gameState.tileGrid[i].GetPositionX() + 40);
				int posY = (int)(gameState.tileGrid[i].GetPositionY() + 40);

				SDL_Rect biscuitRect = { posX, posY, 20, 20 };
				SDL_RenderCopy(renderer, gameState.biscuitTexture, NULL, &biscuitRect);
			}
		}

		for (int i = 0; i < 4; i++) {
			gameState.enemySprites[i].Render(renderer);
		}

		gameState.playerSprite.Render(renderer);
		gameState.playerScoreText.Render(renderer);

		if (gameState.GetState() == TwoPlayer) {
			gameState.playerTwoSprite.Render(renderer);
			gameState.playerTwoScoreText.Render(renderer);
		}
	}

	// Finalise Render
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderPresent(renderer);
}

void Reset() {
	gameState.playerSprite.alive = true;
	gameState.playerSprite.score = 0;
	gameState.playerSprite.tile = Globals::PLAYER_START_X + (Globals::PLAYER_START_Y * Globals::TILE_ROWS);
	gameState.playerSprite.SetPositionFromTile(gameState);
	gameState.playerSprite.moveDirection = Left;

	gameState.playerTwoSprite.alive = true;
	gameState.playerTwoSprite.score = 0;
	gameState.playerTwoSprite.tile = Globals::PLAYER_START_X + (Globals::PLAYER_START_Y * Globals::TILE_ROWS);
	gameState.playerTwoSprite.SetPositionFromTile(gameState);
	gameState.playerTwoSprite.moveDirection = Right;

	for (int i = 0; i < 4; i++) {
		gameState.enemySprites[i].alive = true;
		gameState.enemySprites[i].tile = Globals::ENEMY_START_X[i] + (Globals::ENEMY_START_Y[i] * Globals::TILE_ROWS);
		gameState.enemySprites[i].SetPositionFromTile(gameState);
		gameState.enemySprites[i].moveDirection = Left;
	}

	for (int i = 0; i < Globals::TILE_COUNT; i++) {
		if (gameState.tileGrid[i].GetState() == -1) {
			gameState.tileGrid[i].Reset();
		}
	}
}
