//
// Copyright (c) 2016 by Ben Townshend. All Rights Reserved.
//

#pragma once

#include "Sprite.h"
#include <string>

class FontSprite : public Sprite {
public:
	std::string text;

	void Render(SDL_Renderer*);
	int getFontRow(char);
	int getFontColumn(char);
};
