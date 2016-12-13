//
// Copyright (c) 2016 by Ben Townshend. All Rights Reserved.
//

#pragma once

#include <string>
#include <functional>
#include "Sprite.h"

class FontSprite : public Sprite {
public:
	std::string text = "";
	bool canSelect = true;
	bool canClick = true;
	bool selected = false;
	double fontScale;
	SDL_Texture* selectedTexture;
	std::function<void()> onClick;

	FontSprite() {}
	FontSprite(std::string, SDL_Texture*, SDL_Texture*, int, int, double, bool, bool);
	FontSprite(std::string, SDL_Texture*, SDL_Texture*, int, int, double, bool, bool, std::function<void()>);
	void CentreHorizontal();
	void DoClick();
	bool CheckBounds(int, int);
	void Render(SDL_Renderer*);
	int getFontRow(char);
	int getFontColumn(char);
};
