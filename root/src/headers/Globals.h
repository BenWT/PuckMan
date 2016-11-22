//
// Copyright (c) 2016 by Ben Townshend. All Rights Reserved.
//

#pragma once

#define N_TILES 7
#define TILE_SIZE 100
#define SCORE_REGION 0
#define SCREEN_WIDTH TILE_SIZE * N_TILES
#define SCREEN_HEIGHT SCREEN_WIDTH + SCORE_REGION
#define PLAYER_START_X 1
#define PLAYER_START_Y 1

int GRID_TYPE[] {
	6, 2, 7, 0, 6, 2, 7,
	1, 0, 3, 0, 1, 0, 3,
	1, 0, 9, 2, 12, 0, 3,
	1, 0, 0, 0, 0, 0, 3,
	1, 0, 10, 4, 11, 0, 3,
	1, 0, 3, 0, 1, 0, 3,
	5, 4, 8, 0, 5, 4, 8
};
