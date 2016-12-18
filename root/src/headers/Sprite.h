//
// Copyright (c) 2016 by Ben Townshend. All Rights Reserved.
//

#pragma once

#include <string>
#include <vector>
#include "SDL.h"
#include "Vector2.h"

class Sprite {
public:
	Vector2* position;
	Vector2* scale;
	SDL_Texture* texture;
	SDL_Rect srcRect;
	SDL_Rect destRect;
	bool doAnimate = false;
	int currentAnim = 0;
	double frameTime = 1.0;
	std::vector<SDL_Rect> animStates;

	void NormalisePosition();
	void CalculateRect();
	void SetTexture(SDL_Texture*);
	void CycleAnims(double);
	void Render(SDL_Renderer*);

	Sprite();
	Sprite(SDL_Texture*, SDL_Rect, Vector2*);
	Sprite(SDL_Texture*, SDL_Rect, Vector2*, Vector2*);
private:
	double animCounter = 0.0;
};
