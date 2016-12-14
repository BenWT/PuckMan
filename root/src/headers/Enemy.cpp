//
// Copyright (c) 2016 by Ben Townshend. All Rights Reserved.
//

#include <iostream>
#include "Globals.h"
#include "GameState.h"
#include "Tile.h"
#include "Enemy.h"

void Enemy::Render(SDL_Renderer* renderer) {
    SDL_Rect drawRect = destRect;
    drawRect.x += (int)offsetX;
    drawRect.y += (int)offsetY;
    SDL_RenderCopy(renderer, texture, (srcRect.w == -1) ? NULL : &srcRect, &drawRect);

    //std::cout << drawRect.x << " " << drawRect.y << std::endl;
}

void Enemy::SetPositionFromTile(GameState& gameState) {
	position = gameState.tileGrid[tile].GetPosition();
	CalculateRect();
}
