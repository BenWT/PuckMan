//
// Copyright (c) 2016 by Ben Townshend. All Rights Reserved.
//

#pragma once

#include "Sprite.h"
#include <string>

class FontSprite : public Sprite {
public:
	std::string text = "";
	bool canSelect = true;
	bool canClick = true;
	bool selected = false;
	bool clicked = false;
	double fontScale;
	SDL_Texture* selectedTexture;

	FontSprite() {}
	FontSprite(std::string, SDL_Texture*, SDL_Texture*, int, int, double, bool, bool);
	void CentreHorizontal();
	void DoClick();
	bool CheckBounds(int, int);
	void Render(SDL_Renderer*);
	int getFontRow(char);
	int getFontColumn(char);
};
