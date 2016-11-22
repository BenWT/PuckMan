//
// Copyright (c) 2016 by Ben Townshend. All Rights Reserved.
//

#pragma once

class Globals {
public:
    const static int TILE_ROWS = 7;
    const static int TILE_COUNT = TILE_ROWS * TILE_ROWS;
    const static int TILE_SIZE = 100;
    const static int SCORE_REGION = 0;
    const static int SCREEN_WIDTH = TILE_SIZE * TILE_ROWS;
    const static int SCREEN_HEIGHT = SCREEN_WIDTH + SCORE_REGION;
    const static int PLAYER_START_X = 1;
    const static int PLAYER_START_Y = 1;
    const static int GRID_TYPE[];
};

/*
static int TILE_ROWS;
static int TILE_COUNT;
static int TILE_SIZE;
static int SCORE_REGION;
static int SCREEN_WIDTH;
static int SCREEN_HEIGHT;
static int PLAYER_START_X;
static int PLAYER_START_Y;
static int GRID_TYPE[];
*/
