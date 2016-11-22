//
// Copyright (c) 2016 by Ben Townshend. All Rights Reserved.
//

#pragma once

#include <string>
#include "SDL.h"
//#include "Globals.h"
#include "Vector2.h"

class Sprite {
public:
	Vector2* position;
	Vector2* scale;
	SDL_Texture* texture;
	SDL_Rect srcRect;
	SDL_Rect destRect;

	void CalculateRect();
	void SetTexture(SDL_Texture*);
	void Render(SDL_Renderer*);

	Sprite();
	Sprite(SDL_Texture*, SDL_Rect, Vector2*);
	Sprite(SDL_Texture*, SDL_Rect, Vector2*, Vector2*);
};
