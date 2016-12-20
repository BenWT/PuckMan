//
// Copyright (c) 2016 by Ben Townshend. All Rights Reserved.
//

#pragma once

enum MenuState { MainMenu, OnePlayer, TwoPlayer, EndGameOnePlayer, EndGameTwoPlayer };
enum MoveDirection { Up, Down, Left, Right };

class Globals {
public:
    const static int TILE_ROWS = 37;
    const static int TILE_COUNT = TILE_ROWS * TILE_ROWS;
    const static int TILE_SIZE = 50;
    const static int TOTAL_SCORE = 14200;
    const static int SCORE_REGION = 100;
    const static int ACTUAL_SCREEN_WIDTH = 800;
    const static int ACTUAL_SCREEN_HEIGHT = 600;
    const static int SCREEN_WIDTH = TILE_SIZE * TILE_ROWS;
    const static int SCREEN_HEIGHT = SCREEN_WIDTH + SCORE_REGION;
    const static int PLAYER_START_X = 18;
    const static int PLAYER_START_Y = 25;
	const static int PLAYER_SPEED = 200;
    const static int PLAYER_LIVES = 3;
	const static int BISCUIT_SCORE = 50;
	const static int FONT_WIDTH = 9;
    const static int FONT_HEIGHT = 13;
    const static int GRID_TYPE[];
    const static int ENEMY_START_X[];
    const static int ENEMY_START_Y[];

	const static int MAIN_MENU_ITEMS = 5;
    const static int END_GAME_ONE_ITEMS = 3;
    const static int END_GAME_TWO_ITEMS = 3;

	const static int JOYSTICK_DEAD_ZONE = 8000;
};
