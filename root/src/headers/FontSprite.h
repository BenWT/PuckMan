//
// Copyright (c) 2016 by Ben Townshend. All Rights Reserved.
//

#pragma once

#include "Sprite.h"
#include <string>

class FontSprite : public Sprite {
public:
	bool selected = false;
	std::string text = "";
	int fontSize;
	SDL_Texture* selectedTexture;

	void Render(SDL_Renderer*);
	int getFontRow(char);
	int getFontColumn(char);
};
