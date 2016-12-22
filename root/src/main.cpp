//
// Copyright (c) 2016 by Ben Townshend. All Rights Reserved.
//

#include "headers/Globals.h"

#include <iostream>
#include <chrono>
#include <string>
#include <cmath>
#include <fstream>
#include <sstream>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "rapidjson/document.h"
#include "headers/GameState.h"
#include "headers/Tile.h"
#include "headers/Vector2.h"
#include "headers/Sprite.h"
#include "headers/Player.h"
#include "headers/Enemy.h"
#include "headers/FontSprite.h"

using namespace std;
using namespace chrono;
using namespace rapidjson;

// Methods
void InitialiseSprites();
void LoadAudio();
void GetKeybinding();
void ProcessInput();
void Update(double&);
void Render();
void Reset();
void toggleFullScreen(SDL_Window* window)
{
	Uint32 FullscreenFlag = SDL_WINDOW_FULLSCREEN_DESKTOP;
	bool toggle = SDL_GetWindowFlags(window) & FullscreenFlag;
	SDL_SetWindowFullscreen(window, toggle ? 0 : FullscreenFlag);
}
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
	#ifdef _WIN32 // Windows reverses X and Y axis of right joystick
		return basePath + path;
	#else
		return path;
	#endif
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
	Mix_HaltMusic();
}
void twoPlayerCallback() {
	Reset();
	gameState.SetState(TwoPlayer);
	Mix_HaltMusic();
}
void optionsCallback() {
	gameState.SetState(Options);
}
void quitCallback() {
	SDL_Log("Program quit.");
	running = false;
}
void retryOnePlayerCallback() {
	Reset();
	gameState.SetState(OnePlayer);
	Mix_HaltMusic();
}
void retryTwoPlayerCallback() {
	Reset();
	gameState.SetState(TwoPlayer);
	Mix_HaltMusic();
}
void backToMenuCallback() {
	gameState.SetState(MainMenu);
}
void increaseVolumeCallback() {
	gameState.musicVolume += 0.1;
	if (gameState.musicVolume >= 1.0) gameState.musicVolume = 1.0;
}
void decreaseVolumeCallback() {
	gameState.musicVolume -= 0.1;
	if (gameState.musicVolume <= 0.0) gameState.musicVolume = 0.0;
}
void resumeCallback() {
	gameState.paused = false;
}
void backToMenuPausedCallback() {
	Reset();
	Mix_HaltMusic();
	gameState.paused = false;
	gameState.SetState(MainMenu);
}

int main(int argc, char *argv[]) {
	// Get Base Path
	basePath = argv[0];
	basePath = GetPathFromFullPath(basePath).c_str();
	cout << basePath << endl;

	// Initialise SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "SDL failed to initialise. \n");
		return 1;
	}
	SDL_Log("SDL initialised successfully. \n");

	// Initialise SDL_image
	int imgFlags = IMG_INIT_PNG;
	if( !( IMG_Init( imgFlags ) & imgFlags ) ) {
		SDL_LogCritical(SDL_LOG_CATEGORY_VIDEO, "SDL_image failed to initialise. \n");
		return 1;
	}
	SDL_Log("SDL_image initialised successfully. \n");

	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
		SDL_LogCritical(SDL_LOG_CATEGORY_AUDIO, "SDL_mixer failed to initialise. \n");
		return 1;
	}
	SDL_Log("SDL_mixer initialised successfully. \n");

	// Create Window
	SDL_DisplayMode display;
	SDL_GetCurrentDisplayMode(0, &display);
	int x = display.w, y = display.h;

	SDL_CreateWindowAndRenderer(x / 2, y / 2, SDL_WINDOW_RESIZABLE, &window, &renderer);
	if (window == NULL || renderer == NULL) {
		SDL_LogCritical(SDL_LOG_CATEGORY_VIDEO, "SDL failed to create a window. \n");
		return 1;
	}
	SDL_RenderSetLogicalSize(renderer, Globals::SCREEN_WIDTH, Globals::SCREEN_HEIGHT);
	SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	SDL_SetWindowTitle(window, "PuckMan - Ben Townshend - 13480634 - CGP2011M");

	// Create Gamepad
	for (int i = 0; i < SDL_NumJoysticks(); i++) {
		joystick = SDL_JoystickOpen(i);
	   	if (joystick) {
			SDL_Log("Joystick initialised successfully. \n");
		} else {
			SDL_LogCritical(SDL_LOG_CATEGORY_INPUT, "SDL failed to load joystick. \n");
		}
	}

	// Create Sprites
	InitialiseSprites();
	LoadAudio();
	GetKeybinding();

	// Game Loop
	running = true;
	high_resolution_clock::time_point frameTime = NowTime(), updateStart = NowTime(), renderStart = NowTime();
	double deltaTime = 0, inputTime = 0, updateTime = 0, renderTime = 0;
	string performanceInfo;



	while (running) {
		deltaTime =  TimeSinceLastFrame(frameTime);
		frameTime = NowTime();


		ProcessInput();
		inputTime = TimeSinceLastFrame(frameTime);
		inputTime *= 1000;

		updateStart = NowTime();
		Update(deltaTime);
		updateTime = TimeSinceLastFrame(updateStart);
		updateTime *= 1000;

		renderStart = NowTime();
		Render();
		renderTime = TimeSinceLastFrame(renderStart);
		renderTime *= 1000;

		performanceInfo = "ProcessInput: " + to_string(inputTime) + "ms " + "Update: " + to_string(updateTime) + "ms " + "Render: " + to_string(renderTime) + "ms";
		gameState.renderInfo.ChangeText(performanceInfo);
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
	SDL_Surface* playerSurface = IMG_Load(AddBase("assets/textures/puckman.png").c_str());
	SDL_Surface* enemySurface = IMG_Load(AddBase("assets/textures/ghost.png").c_str());
	SDL_Surface* tileSurface = IMG_Load(AddBase("assets/textures/tiles.png").c_str());
	SDL_Surface* biscuitSurface = IMG_Load(AddBase("assets/textures/biscuit.png").c_str());
	SDL_Surface* pillSurface = IMG_Load(AddBase("assets/textures/pill.png").c_str());
	SDL_Surface* heartSurface = IMG_Load(AddBase("assets/textures/heart.png").c_str());
	SDL_Surface* pauseSurface = IMG_Load(AddBase("assets/textures/pause.png").c_str());
	SDL_Surface* fontSurface = IMG_Load(AddBase("assets/textures/fonts.png").c_str());
	SDL_Surface* fontSelectedSurface = IMG_Load(AddBase("assets/textures/fontsSelected.png").c_str());

	// Textures
	SDL_Texture* playerTexture = SDL_CreateTextureFromSurface(renderer, playerSurface);
	SDL_Texture* enemyTexture = SDL_CreateTextureFromSurface(renderer, enemySurface);
	SDL_Texture* tileTexture = SDL_CreateTextureFromSurface(renderer, tileSurface);
	gameState.biscuitTexture = SDL_CreateTextureFromSurface(renderer, biscuitSurface);
	gameState.pillTexture = SDL_CreateTextureFromSurface(renderer, pillSurface);
	gameState.heartTexture = SDL_CreateTextureFromSurface(renderer, heartSurface);
	gameState.pauseTexture = SDL_CreateTextureFromSurface(renderer, pauseSurface);
	SDL_Texture* font = SDL_CreateTextureFromSurface(renderer, fontSurface);
	SDL_Texture* fontS = SDL_CreateTextureFromSurface(renderer, fontSelectedSurface);

	// Loading Screen
	FontSprite* loadingScreenTitle = new FontSprite("PuckMan", font, fontS, 0, 250, 30, false, false, false);
	FontSprite* loadingScreenText = new FontSprite("Loading...", font, fontS, 0, 1250, 10, false, false, false);
	loadingScreenTitle->CentreHorizontal();
	loadingScreenText->CentreHorizontal();
	gameState.loadingScreenText.push_back(*loadingScreenTitle);
	gameState.loadingScreenText.push_back(*loadingScreenText);
	delete loadingScreenTitle;
	delete loadingScreenText;

	// Main Menu
	int mainMenuScale = 10;
	FontSprite* title = new FontSprite("PuckMan", font, fontS, 0, 100, mainMenuScale * 2, false, false, false);
	FontSprite* onePlayer = new FontSprite("One Player", font, fontS, 0, 150 + mainMenuScale * Globals::FONT_HEIGHT * 2, mainMenuScale, true, true, true, onePlayerCallback);
	FontSprite* twoPlayer = new FontSprite("Two Player", font, fontS, 0, 160 + mainMenuScale * Globals::FONT_HEIGHT * 3, mainMenuScale, true, true, true, twoPlayerCallback);
	FontSprite* options = new FontSprite("Options", font, fontS, 0, 170 + mainMenuScale * Globals::FONT_HEIGHT * 4, mainMenuScale, true, true, true, optionsCallback);
	FontSprite* quit = new FontSprite("Quit", font, fontS, 0, 180 + mainMenuScale * Globals::FONT_HEIGHT * 5, mainMenuScale, true, true, true, quitCallback);
	title->CentreHorizontal();
	onePlayer->CentreHorizontal();
	twoPlayer->CentreHorizontal();
	options->CentreHorizontal();
	quit->CentreHorizontal();
	gameState.mainMenuText.push_back(*title);
	gameState.mainMenuText.push_back(*onePlayer);
	gameState.mainMenuText.push_back(*twoPlayer);
	gameState.mainMenuText.push_back(*options);
	gameState.mainMenuText.push_back(*quit);
	delete title;
	delete onePlayer;
	delete twoPlayer;
	delete options;
	delete quit;

	// One Player Death Menu
	FontSprite* gameOver = new FontSprite("Game Over!", font, fontS, 0, 100, mainMenuScale * 2, false, false, false);
	FontSprite* scoreOne = new FontSprite("Score: ", font, fontS, 0,  150 + mainMenuScale * Globals::FONT_HEIGHT * 2, mainMenuScale, false, false, false);
	FontSprite* scoreTwo = new FontSprite("Score: ", font, fontS, 0, 160 + mainMenuScale * Globals::FONT_HEIGHT * 3, mainMenuScale, false, false, false);
	FontSprite* retryOnePlayer = new FontSprite("Retry?", font, fontS, 0, Globals::SCREEN_HEIGHT - (2 * Globals::FONT_HEIGHT * mainMenuScale), mainMenuScale, true, true, true, retryOnePlayerCallback);
	FontSprite* retryTwoPlayer = new FontSprite("Retry?", font, fontS, 0, Globals::SCREEN_HEIGHT - (2 * Globals::FONT_HEIGHT * mainMenuScale), mainMenuScale, true, true, true, retryTwoPlayerCallback);
	FontSprite* mainMenu = new FontSprite("Back to Menu", font, fontS, 0, Globals::SCREEN_HEIGHT - (Globals::FONT_HEIGHT * mainMenuScale), mainMenuScale, true, true, true, backToMenuCallback);
	gameOver->CentreHorizontal();
	scoreOne->CentreHorizontal();
	scoreTwo->CentreHorizontal();
	retryOnePlayer->CentreHorizontal();
	retryTwoPlayer->CentreHorizontal();
	mainMenu->CentreHorizontal();
	gameState.endGameOneText.push_back(*gameOver);
	gameState.endGameOneText.push_back(*scoreOne);
	gameState.endGameOneText.push_back(*retryOnePlayer);
	gameState.endGameOneText.push_back(*mainMenu);
	gameState.endGameTwoText.push_back(*gameOver);
	gameState.endGameTwoText.push_back(*scoreOne);
	gameState.endGameTwoText.push_back(*scoreTwo);
	gameState.endGameTwoText.push_back(*retryTwoPlayer);
	gameState.endGameTwoText.push_back(*mainMenu);
	delete gameOver;
	delete scoreOne;
	delete scoreTwo;
	delete retryOnePlayer;
	delete retryTwoPlayer;

	// Options Menu
	FontSprite* optionsTitle = new FontSprite("Options", font, fontS, 0, 100, mainMenuScale * 2, false, false, false);
	FontSprite* musicVolume = new FontSprite("Music Volume", font, fontS, 0, 150 + mainMenuScale * Globals::FONT_HEIGHT * 2, mainMenuScale, false, false, false);
	FontSprite* increaseVolume = new FontSprite("+", font, fontS, Globals::SCREEN_WIDTH - (Globals::FONT_WIDTH * mainMenuScale * 3), 150 + mainMenuScale * Globals::FONT_HEIGHT * 2, mainMenuScale, true, true, false, increaseVolumeCallback);
	FontSprite* decreaseVolume = new FontSprite("-", font, fontS, Globals::SCREEN_WIDTH - (Globals::FONT_WIDTH * mainMenuScale), 150 + mainMenuScale * Globals::FONT_HEIGHT * 2, mainMenuScale, true, true, false, decreaseVolumeCallback);
	optionsTitle->CentreHorizontal();
	gameState.optionsText.push_back(*optionsTitle);
	gameState.optionsText.push_back(*musicVolume);
	gameState.optionsText.push_back(*increaseVolume);
	gameState.optionsText.push_back(*decreaseVolume);
	gameState.optionsText.push_back(*mainMenu);
	delete optionsTitle;
	delete increaseVolume;
	delete decreaseVolume;
	delete mainMenu;

	// Pause Menu
	FontSprite* renderInfo = new FontSprite("", font, fontS, 0, 0, mainMenuScale / 5, false, false, false);
	gameState.renderInfo = *renderInfo;

	FontSprite* pauseTitle = new FontSprite("Pause", font, fontS, 0, 100, mainMenuScale * 2, false, false, false);
	FontSprite* resumeGame = new FontSprite("Resume", font, fontS, 0, Globals::SCREEN_HEIGHT - (2 * Globals::FONT_HEIGHT * mainMenuScale), mainMenuScale, true, true, true, resumeCallback);
	FontSprite* mainMenuPause = new FontSprite("Back to Menu", font, fontS, 0, Globals::SCREEN_HEIGHT - (Globals::FONT_HEIGHT * mainMenuScale), mainMenuScale, true, true, true, backToMenuPausedCallback);
	pauseTitle->CentreHorizontal();
	resumeGame->CentreHorizontal();
	mainMenuPause->CentreHorizontal();
	gameState.pauseText.push_back(*pauseTitle);
	gameState.pauseText.push_back(*resumeGame);
	gameState.pauseText.push_back(*mainMenuPause);
	delete renderInfo;
	delete pauseTitle;
	delete resumeGame;
	delete mainMenuPause;

	// Score Text
	FontSprite* playerOneScoreText = new FontSprite("Score: ", font, fontS, (Globals::TILE_SIZE * 5) + 80,  Globals::SCREEN_HEIGHT - (Globals::FONT_HEIGHT * 5), 5, false, false, false);
	FontSprite* playerTwoScoreText = new FontSprite("Score: ", font, fontS, (Globals::SCREEN_WIDTH / 2) + Globals::TILE_SIZE, Globals::SCREEN_HEIGHT - (Globals::FONT_HEIGHT * 5), 5, false, false, false);
	gameState.playerScoreText = *playerOneScoreText;
	gameState.playerTwoScoreText = *playerTwoScoreText;
	delete playerOneScoreText;
	delete playerTwoScoreText;

	// Tile Textures
	for (int i = 0; i < Globals::TILE_COUNT; i++) {
		Tile t = gameState.tileGrid[i];
		Sprite* s = new Sprite(
			tileTexture,
			NewRect((int)(t.GetTextureX()), (int)(t.GetTextureY()), 100, 100),
			new Vector2((int)(t.GetPositionX()), (int)(t.GetPositionY())),
			new Vector2(Globals::TILE_SIZE, Globals::TILE_SIZE)
		);
		s->CalculateRect();
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
			e->animStates.push_back(NewRect(j * 100, i * 100, 100, 100));
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
		p2->animStates.push_back(NewRect(i * 100, 100, 100, 100));
	}
	gameState.playerTwoSprite = *p2;
	delete p2;
}

void LoadAudio() {
	gameState.menuMusic = Mix_LoadMUS(AddBase("assets/audio/Puckman_Menu_Theme.mp3").c_str());
	gameState.gameMusic = Mix_LoadMUS(AddBase("assets/audio/Puckman_Game_Theme.mp3").c_str());
	gameState.clickSound = Mix_LoadWAV(AddBase("assets/audio/Menu_Click.wav").c_str());
	gameState.biscuitSound = Mix_LoadWAV(AddBase("assets/audio/Pickup_Biscuit.wav").c_str());
	gameState.pillSound = Mix_LoadWAV(AddBase("assets/audio/Pickup_Pill.wav").c_str());
	gameState.hitSound = Mix_LoadWAV(AddBase("assets/audio/Hit.wav").c_str());
	gameState.ghostDeathSound = Mix_LoadWAV	(AddBase("assets/audio/Ghost_Death.wav").c_str());

	if (gameState.menuMusic == NULL ||
		gameState.gameMusic == NULL ||
		gameState.clickSound == NULL ||
		gameState.biscuitSound == NULL ||
		gameState.pillSound == NULL ||
		gameState.hitSound == NULL ||
		gameState.ghostDeathSound == NULL) {
		SDL_LogCritical(SDL_LOG_CATEGORY_AUDIO, "Audio files could not be loaded. \n");
	}
}

void GetKeybinding() {
	ifstream file(AddBase("assets/keybinding.json").c_str());
    stringstream ss;

    ss << file.rdbuf();
	string in = ss.str();
    const char* json = in.c_str();

	if (strlen(json) > 0) {
		string in = ss.str();
	    const char* json = in.c_str();

		Document d;
	    d.Parse(json);

		gameState.playerOneUpKey = SDL_GetKeyFromName(d["playerOneUpKey"].GetString());
		gameState.playerOneDownKey = SDL_GetKeyFromName(d["playerOneDownKey"].GetString());;
		gameState.playerOneLeftKey = SDL_GetKeyFromName(d["playerOneLeftKey"].GetString());;
		gameState.playerOneRightKey = SDL_GetKeyFromName(d["playerOneRightKey"].GetString());;
		gameState.playerTwoUpKey = SDL_GetKeyFromName(d["playerTwoUpKey"].GetString());;
		gameState.playerTwoDownKey = SDL_GetKeyFromName(d["playerTwoDownKey"].GetString());;
		gameState.playerTwoLeftKey = SDL_GetKeyFromName(d["playerTwoLeftKey"].GetString());;
		gameState.playerTwoRightKey = SDL_GetKeyFromName(d["playerTwoRightKey"].GetString());;
		gameState.selectKey = SDL_GetKeyFromName(d["selectKey"].GetString());
		SDL_Log("Keybindings were loaded from file.");
	} else {
		gameState.playerOneUpKey = SDLK_w;
		gameState.playerOneDownKey = SDLK_s;
		gameState.playerOneLeftKey = SDLK_a;
		gameState.playerOneRightKey = SDLK_d;
		gameState.playerTwoUpKey = SDLK_UP;
		gameState.playerTwoDownKey = SDLK_DOWN;
		gameState.playerTwoLeftKey = SDLK_LEFT;
		gameState.playerTwoRightKey = SDLK_RIGHT;
		gameState.selectKey = SDLK_RETURN;
		SDL_Log("Keybindings could not be loaded from file.");
	}
}

void ProcessInput() {
	gameState.ResetInputs();
	//SDL_GetKeyFromName()

	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		SDL_Keycode k = event.key.keysym.sym;

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
			if (k == gameState.playerOneUpKey) {
					gameState.w = true;
				} else if (k == gameState.playerOneLeftKey) {
					gameState.a = true;
				} else if (k == gameState.playerOneDownKey) {
					gameState.s = true;
				} else if (k == gameState.playerOneRightKey) {
					gameState.d = true;
				} else if (k == gameState.playerTwoUpKey) {
					gameState.up = true;
				} else if (k == gameState.playerTwoLeftKey) {
					gameState.left = true;
				} else if (k == gameState.playerTwoDownKey) {
					gameState.down = true;
				} else if (k == gameState.playerTwoRightKey) {
					gameState.right = true;
				} else if (k == gameState.selectKey) {
					gameState.enter = true;
				} else if (k == SDLK_ESCAPE) {
					if (gameState.GetState() == MainMenu) {
						SDL_Log("Program quit.");
			 		    running = false;
					} else if (gameState.GetState() == EndGameOnePlayer || gameState.GetState() == EndGameTwoPlayer || gameState.GetState() == Options) {
						backToMenuCallback();
					} else if (gameState.GetState() == OnePlayer || gameState.GetState() == TwoPlayer) {
						gameState.paused = !gameState.paused;
					}
				} else if (k == SDLK_p) {
					toggleFullScreen(window);
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
	if (gameState.GetState() == Loading) {
		gameState.loadingScreenTimer += deltaTime;

		if (gameState.loadingScreenTimer >= 1.5) {
			gameState.SetState(MainMenu);
		}
	} else {
		double p1Speed = deltaTime * Globals::PLAYER_SPEED;
		double p2Speed = deltaTime * Globals::PLAYER_SPEED;
		bool selectedWithMouse = false;
		bool p1Up = false, p1Left = false, p1Down = false, p1Right = false;
		bool p2Up = false, p2Left = false, p2Down = false, p2Right = false;

		if (gameState.GetState() == MainMenu || gameState.GetState() == EndGameOnePlayer || gameState.GetState() == EndGameTwoPlayer || gameState.GetState() == Options || gameState.paused) {
			int* index = &gameState.mainMenuSelectionIndex;
			vector<FontSprite>* items = &gameState.mainMenuText;
			vector<FontSprite>::iterator it;

			if (gameState.GetState() == MainMenu) {
				index = &gameState.mainMenuSelectionIndex;
				items = &gameState.mainMenuText;
			} else if (gameState.GetState() == EndGameOnePlayer) {
				index = &gameState.endGameOneSelectionIndex;
				items = &gameState.endGameOneText;

				gameState.endGameOneText[1].ChangeText("Score: " + to_string(gameState.playerSprite.score));
				gameState.endGameOneText[1].CentreHorizontal();
			} else if (gameState.GetState() == EndGameTwoPlayer) {
				index = &gameState.endGameTwoSelectionIndex;
				items = &gameState.endGameTwoText;

				gameState.endGameTwoText[1].ChangeText("Score: " + to_string(gameState.playerSprite.score));
				gameState.endGameTwoText[1].CentreHorizontal();
				gameState.endGameTwoText[2].ChangeText("Score: " + to_string(gameState.playerTwoSprite.score));
				gameState.endGameTwoText[2].CentreHorizontal();
			} else if (gameState.GetState() == Options) {
				index = &gameState.optionsSelectionIndex;
				items = &gameState.optionsText;

				gameState.optionsText[1].ChangeText("Volume: " + to_string((int)(gameState.musicVolume * 100)) + "%");
			}
			if (gameState.paused && (gameState.GetState() == OnePlayer || gameState.GetState() == TwoPlayer)) {
				index = &gameState.pauseSelectionIndex;
				items = &gameState.pauseText;
			}

			for (it = items->begin(); it < items->end(); it++) {
				if (it->canSelect) {
					if (it->CheckBounds(gameState.mouseX, gameState.mouseY)) {
						it->selected = true;
						*index = distance(items->begin(), it);
						selectedWithMouse = true;
					} else {
						it->selected = false;
					}
				}

				if (gameState.mouseClicked) {
					if (it->canClick) {
						if (it->CheckBounds(gameState.mouseX, gameState.mouseY)) {
							it->DoClick();
							gameState.PlayClick();
						}
					}
				}
			}

			if (!selectedWithMouse) {
				bool moveUp = false; bool moveDown = false;

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
					int newIndex = *index - 1;
					if (newIndex >= 0 && items->at(newIndex).canSelect) *index = newIndex;
				} else if (moveDown) {
					int newIndex = *index + 1;
					if (newIndex < items->size()) {
						if (items->at(newIndex).canSelect) *index = newIndex;
					}
				}

				for (it = items->begin(); it < items->end(); it++) {
					if (it->canSelect) {
						it->selected = (distance(items->begin(), it) == *index);

						if (*index == distance(items->begin(), it)) {
							it->selected = true;

							if (gameState.enter || gameState.aGamePad) {
								if (it->canClick) {
									it->DoClick();
									gameState.PlayClick();
								}
							}
						}
					}
				}
			}

			gameState.joystickTimer += deltaTime;
		} else if (gameState.GetState() == OnePlayer) {
			if (!gameState.paused) {
				// Player One
				if (gameState.w || gameState.up || gameState.leftJoystickY < -Globals::JOYSTICK_DEAD_ZONE) p1Up = true;
				else if (gameState.a || gameState.left || gameState.leftJoystickX < -Globals::JOYSTICK_DEAD_ZONE) p1Left = true;
				else if (gameState.s || gameState.down || gameState.leftJoystickY > Globals::JOYSTICK_DEAD_ZONE) p1Down = true;
				else if (gameState.d || gameState.right || gameState.leftJoystickX > Globals::JOYSTICK_DEAD_ZONE) p1Right = true;
			}
		} else if (gameState.GetState() == TwoPlayer) {
			if (!gameState.paused) {
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
		}

		if (gameState.GetState() == OnePlayer || gameState.GetState() == TwoPlayer) {
			if (!gameState.paused) {
				gameState.pillAngle += deltaTime * 15;
				if (gameState.pillAngle >= 360.0) gameState.pillAngle -= 360.0;

				if (gameState.playerSprite.alive) {
					if (gameState.GetState() == TwoPlayer) {
						if (!gameState.playerTwoSprite.hasPill) {
							if (p1Up && gameState.playerSprite.CanMove(gameState, Up, deltaTime)) gameState.playerSprite.moveDirection = Up;
							if (p1Left && gameState.playerSprite.CanMove(gameState, Left, deltaTime)) gameState.playerSprite.moveDirection = Left;
							if (p1Down && gameState.playerSprite.CanMove(gameState, Down, deltaTime)) gameState.playerSprite.moveDirection = Down;
							if (p1Right && gameState.playerSprite.CanMove(gameState, Right, deltaTime)) gameState.playerSprite.moveDirection = Right;
						}
					} else {
						if (p1Up && gameState.playerSprite.CanMove(gameState, Up, deltaTime)) gameState.playerSprite.moveDirection = Up;
						if (p1Left && gameState.playerSprite.CanMove(gameState, Left, deltaTime)) gameState.playerSprite.moveDirection = Left;
						if (p1Down && gameState.playerSprite.CanMove(gameState, Down, deltaTime)) gameState.playerSprite.moveDirection = Down;
						if (p1Right && gameState.playerSprite.CanMove(gameState, Right, deltaTime)) gameState.playerSprite.moveDirection = Right;
					}
				}

				if (gameState.playerTwoSprite.alive && gameState.GetState() == TwoPlayer) {
					if (!gameState.playerSprite.hasPill) {
						if (p2Up && gameState.playerTwoSprite.CanMove(gameState, Up, deltaTime)) gameState.playerTwoSprite.moveDirection = Up;
						if (p2Left && gameState.playerTwoSprite.CanMove(gameState, Left, deltaTime)) gameState.playerTwoSprite.moveDirection = Left;
						if (p2Down && gameState.playerTwoSprite.CanMove(gameState, Down, deltaTime)) gameState.playerTwoSprite.moveDirection = Down;
						if (p2Right && gameState.playerTwoSprite.CanMove(gameState, Right, deltaTime)) gameState.playerTwoSprite.moveDirection = Right;
					}
				}

				for (int i = 0; i < 4; i++) {
					if (gameState.playerSprite.tile == gameState.enemySprites[i].tile) {
						if (gameState.playerSprite.hasPill) {
							gameState.enemySprites[i].Kill(gameState);
							gameState.PlayGhostDeath();
						} else {
							if (gameState.playerSprite.deathTimer >= gameState.playerSprite.deathTime) {
								gameState.playerSprite.lives--;
								gameState.playerSprite.deathTimer = 0;
								gameState.PlayHit();

								if (gameState.playerSprite.lives <= 0) {
									gameState.playerSprite.alive = false;
								}
							}
						}
					}
					if (gameState.playerTwoSprite.tile == gameState.enemySprites[i].tile) {
						if (gameState.GetState() == TwoPlayer) {
							if (gameState.playerTwoSprite.hasPill) {
								gameState.enemySprites[i].Kill(gameState);
								gameState.PlayGhostDeath();
							} else {
								if (gameState.playerTwoSprite.deathTimer >= gameState.playerTwoSprite.deathTime) {
									gameState.playerTwoSprite.lives--;
									gameState.playerTwoSprite.deathTimer = 0;
									gameState.PlayHit();

									if (gameState.playerTwoSprite.lives <= 0) {
										gameState.playerTwoSprite.alive = false;
									}
								}
							}
						}
					}
				}

				if (gameState.GetState() == OnePlayer) {
					if (gameState.playerSprite.score >= Globals::TOTAL_SCORE) {
						gameState.endGameOneText.at(0).ChangeText("Winner!");
						gameState.endGameOneText.at(0).CentreHorizontal();
						gameState.SetState(EndGameOnePlayer);
						Mix_HaltMusic();
					}
					if (!gameState.playerSprite.alive) {
						gameState.endGameOneText.at(0).ChangeText("Game Over!");
						gameState.endGameOneText.at(0).CentreHorizontal();
						gameState.SetState(EndGameOnePlayer);
						Mix_HaltMusic();
					}
				} else if (gameState.GetState() == TwoPlayer) {
					if (gameState.playerSprite.score + gameState.playerTwoSprite.score >= Globals::TOTAL_SCORE) {
						gameState.endGameTwoText.at(0).ChangeText("Winner!");
						gameState.endGameTwoText.at(0).CentreHorizontal();
						gameState.SetState(EndGameTwoPlayer);
						Mix_HaltMusic();
					}
					if (!gameState.playerSprite.alive && !gameState.playerTwoSprite.alive) {
						gameState.endGameTwoText.at(0).ChangeText("Game Over!");
						gameState.endGameTwoText.at(0).CentreHorizontal();
						gameState.SetState(EndGameTwoPlayer);
						Mix_HaltMusic();
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

					gameState.playerSprite.deathTimer += deltaTime;
				    if (gameState.playerSprite.hasPill) gameState.playerSprite.pillTimer += deltaTime;
				    if (gameState.playerSprite.pillTimer >= gameState.playerSprite.pillTime) gameState.playerSprite.hasPill = false;
				}
				else if (gameState.GetState() == TwoPlayer) {
					gameState.playerSprite.DoMove(gameState, p1Speed * leftJoyStickValue, deltaTime);
					gameState.playerTwoSprite.DoMove(gameState, p2Speed * rightJoyStickValue, deltaTime);

					gameState.playerSprite.deathTimer += deltaTime;
				    if (gameState.playerSprite.hasPill) gameState.playerSprite.pillTimer += deltaTime;
				    if (gameState.playerSprite.pillTimer >= gameState.playerSprite.pillTime) gameState.playerSprite.hasPill = false;

					gameState.playerScoreText.ChangeText("Score: " + to_string(gameState.playerSprite.score));
					gameState.playerTwoScoreText.ChangeText("Score: " + to_string(gameState.playerTwoSprite.score));

					gameState.playerTwoSprite.deathTimer += deltaTime;
				    if (gameState.playerTwoSprite.hasPill) gameState.playerTwoSprite.pillTimer += deltaTime;
				    if (gameState.playerTwoSprite.pillTimer >= gameState.playerTwoSprite.pillTime) gameState.playerTwoSprite.hasPill = false;
				}

				for (int i = 0; i < 4; i++) {
					gameState.enemySprites[i].Roam(gameState, deltaTime);
				}
			}
		}

		if (gameState.GetState() == MainMenu || gameState.GetState() == EndGameOnePlayer || gameState.GetState() == EndGameTwoPlayer || gameState.GetState() == Options) {
			Mix_VolumeMusic(MIX_MAX_VOLUME * gameState.musicVolume);
			if (!Mix_PlayingMusic()) Mix_PlayMusic(gameState.menuMusic, -1);
		} else if(gameState.GetState() == OnePlayer || gameState.GetState() == TwoPlayer) {
			if (gameState.paused) Mix_PauseMusic();
			else Mix_ResumeMusic();

			if (!Mix_PlayingMusic()) Mix_PlayMusic(gameState.gameMusic, -1);
		}
	}
}

void Render() {
	SDL_RenderClear(renderer); // Clear Previous Render

	if (gameState.GetState() == Loading) {
		for (int i = 0; i < gameState.loadingScreenText.size(); i++) {
			gameState.loadingScreenText[i].Render(renderer);
		}
	} else if (gameState.GetState() == MainMenu) {
		for (int i = 0; i < gameState.mainMenuText.size(); i++) {
			gameState.mainMenuText[i].Render(renderer);
		}
	} else if (gameState.GetState() == EndGameOnePlayer) {
		for (int i = 0; i < gameState.endGameOneText.size(); i++) {
			gameState.endGameOneText[i].Render(renderer);
		}
	} else if (gameState.GetState() == EndGameTwoPlayer) {
		for (int i = 0; i < gameState.endGameTwoText.size(); i++) {
			gameState.endGameTwoText[i].Render(renderer);
		}
	} else if (gameState.GetState() == Options) {
		for (int i = 0; i < gameState.optionsText.size(); i++) {
			gameState.optionsText[i].Render(renderer);
		}
	} else if (gameState.GetState() == OnePlayer || gameState.GetState() == TwoPlayer) {

		// Tile Sprites
		for (int i = 0; i < Globals::TILE_COUNT; i++) {
			gameState.tileGrid[i].GetSprite().Render(renderer);

			if (gameState.tileGrid[i].CheckBiscuit()) {
				int size = Globals::TILE_SIZE / 5;
				int posX = (int)(gameState.tileGrid[i].GetPositionX() + (2 * size));
				int posY = (int)(gameState.tileGrid[i].GetPositionY() + (2 * size));

				SDL_Rect biscuitRect = { posX, posY, size, size };
				SDL_RenderCopy(renderer, gameState.biscuitTexture, NULL, &biscuitRect);
			}
			if (gameState.tileGrid[i].CheckPill()) {
				SDL_Rect pillRect = { (int)(gameState.tileGrid[i].GetPositionX()), (int)(gameState.tileGrid[i].GetPositionY()), Globals::TILE_SIZE, Globals::TILE_SIZE };
				SDL_RenderCopyEx(renderer, gameState.pillTexture, NULL, &pillRect, gameState.pillAngle, NULL, SDL_FLIP_NONE);
			}
		}

		for (int i = 0; i < 4; i++) {
			gameState.enemySprites[i].Render(renderer, (gameState.playerSprite.hasPill || gameState.playerTwoSprite.hasPill));
		}

		gameState.playerSprite.Render(renderer);
		gameState.playerScoreText.Render(renderer);
		for (int i = 0; i < gameState.playerSprite.lives; i++) {
			SDL_Rect dest = NewRect((Globals::TILE_SIZE * 5) + 80 + (i * 75), (Globals::TILE_ROWS - 1) * Globals::TILE_SIZE, 65, 65);
			SDL_RenderCopy(renderer, gameState.heartTexture, NULL, &dest);
		}

		if (gameState.GetState() == TwoPlayer) {
			gameState.playerTwoSprite.Render(renderer);
			gameState.playerTwoScoreText.Render(renderer);

			for (int i = 0; i < gameState.playerTwoSprite.lives; i++) {
				SDL_Rect dest = NewRect((Globals::TILE_ROWS * Globals::TILE_SIZE / 2) + Globals::TILE_SIZE + (i * 75), (Globals::TILE_ROWS - 1) * Globals::TILE_SIZE, 65, 65);
				SDL_RenderCopy(renderer, gameState.heartTexture, NULL, &dest);
			}
		}
		if (gameState.paused) {
			SDL_RenderCopy(renderer, gameState.pauseTexture, NULL, NULL);
			for (int i = 0; i < gameState.pauseText.size(); i++) {
				gameState.pauseText[i].Render(renderer);
			}
		}

	}

	// Render performance information to the player
	gameState.renderInfo.Render(renderer);

	// Finalise Render
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderPresent(renderer);
}

void Reset() {
	gameState.playerSprite.alive = true;
	gameState.playerSprite.lives = Globals::PLAYER_LIVES;
	gameState.playerSprite.score = 0;
	gameState.playerSprite.tile = Globals::PLAYER_START_X + (Globals::PLAYER_START_Y * Globals::TILE_ROWS);
	gameState.playerSprite.SetPositionFromTile(gameState);
	gameState.playerSprite.moveDirection = Left;
	gameState.playerSprite.deathTimer = gameState.playerSprite.deathTime;
	gameState.playerSprite.pillTimer = 0.0;

	gameState.playerTwoSprite.alive = true;
	gameState.playerSprite.lives = Globals::PLAYER_LIVES;
	gameState.playerTwoSprite.score = 0;
	gameState.playerTwoSprite.tile = Globals::PLAYER_START_X + (Globals::PLAYER_START_Y * Globals::TILE_ROWS);
	gameState.playerTwoSprite.SetPositionFromTile(gameState);
	gameState.playerTwoSprite.moveDirection = Right;
	gameState.playerTwoSprite.deathTimer = gameState.playerTwoSprite.deathTime;
	gameState.playerTwoSprite.pillTimer = 0.0;

	for (int i = 0; i < 4; i++) {
		gameState.enemySprites[i].alive = true;
		gameState.enemySprites[i].tile = Globals::ENEMY_START_X[i] + (Globals::ENEMY_START_Y[i] * Globals::TILE_ROWS);
		gameState.enemySprites[i].SetPositionFromTile(gameState);
		gameState.enemySprites[i].moveDirection = Left;
	}

	for (int i = 0; i < Globals::TILE_COUNT; i++) {
		if (gameState.tileGrid[i].GetState() == -1 || gameState.tileGrid[i].GetState() == -2) {
			gameState.tileGrid[i].Reset();
		}
	}
}
