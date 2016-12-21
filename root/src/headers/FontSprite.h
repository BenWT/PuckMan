//
// Copyright (c) 2016 by Ben Townshend. All Rights Reserved.
//

#pragma once

#include <string>
#include <functional>
#include "Sprite.h"

class FontSprite : public Sprite {
public:
	bool canSelect = true;
	bool canClick = true;
	bool selected = false;
	bool moveOnSelection = false;
	double fontScale;
	SDL_Texture* selectedTexture;
	std::function<void()> onClick;

	void CentreHorizontal();
	void DoClick();
	bool CheckBounds(int, int);
	void Render(SDL_Renderer*);
	void ChangeText(std::string);
	int getFontRow(char);
	int getFontColumn(char);
	FontSprite() {}
	FontSprite(std::string, SDL_Texture*, SDL_Texture*, int, int, double, bool, bool, bool);
	FontSprite(std::string, SDL_Texture*, SDL_Texture*, int, int, double, bool, bool, bool, std::function<void()>);

private:
	std::string text = "";
};
