//
// Copyright (c) 2016 by Ben Townshend. All Rights Reserved.
//

#ifndef sprite_h
#define sprite_h

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
		int x = position.x - (scale.x / 2);
		int y = position.y - (scale.y / 2);

		destRect.x = x;
		destRect.y = y;
		destRect.w = scale.x;
		destRect.h = scale.y;
	}
	void SetTexture(SDL_Texture* texture) {
		this->texture = texture;
	}

	Sprite() {}
	Sprite(SDL_Texture* texture, SDL_Rect srcRect, Vector2 position) {
		this->texture = texture;
		this->position = position;

		Vector2 scaleV(TILE_SIZE, TILE_SIZE);
		this->scale = scaleV;

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

#endif
