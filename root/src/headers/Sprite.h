//
// Copyright (c) 2016 by Ben Townshend. All Rights Reserved.
//

#pragma once

#include <string>
#include "SDL.h"
#include "Vector2.h"

class Sprite {
public:
	Vector2 position;
	Vector2 scale;
	SDL_Texture* texture;
	SDL_Rect srcRect;
	SDL_Rect destRect;

	void CalculateRect() {
		destRect.x = position.x - (scale.x / 2);
		destRect.y = position.y - (scale.y / 2);
		destRect.w = scale.x;
		destRect.h = scale.y;
	}
	void SetTexture(SDL_Texture* texture) {
		this->texture = texture;
	}
	void Render(SDL_Renderer* renderer) {
		SDL_RenderCopy(renderer, texture, (srcRect.w == -1) ? NULL : &srcRect, &destRect);
	}

	Sprite() {}
	Sprite(SDL_Texture* texture, SDL_Rect srcRect, Vector2 position) {
		this->texture = texture;

		Vector2 scaleV(TILE_SIZE, TILE_SIZE);
		this->scale = scaleV;

		this->position = position;
		this->position.x += (scale.x / 2);
		this->position.y += (scale.y / 2);

		this->srcRect = srcRect;

		CalculateRect();
	}
	Sprite(SDL_Texture* texture, SDL_Rect srcRect, Vector2 position, Vector2 scale) {
		this->texture = texture;
		this->position = position;
		this->scale = scale;
		this->srcRect = srcRect;

		CalculateRect();
	}
private:
};
