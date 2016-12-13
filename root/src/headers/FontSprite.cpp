//
// Copyright (c) 2016 by Ben Townshend. All Rights Reserved.
//

#include <iostream>
#include <string>
#include <functional>
#include "FontSprite.h"
#include "Globals.h"

FontSprite::FontSprite(std::string text, SDL_Texture* tex, SDL_Texture* texS, int x, int y, double fontScale, bool canSelect, bool canClick) {
	SDL_Rect src = {0, 0, Globals::FONT_WIDTH, Globals::FONT_HEIGHT};

	this->text = text;
	this->texture = tex;
	this->selectedTexture = texS;
	this->destRect = { x, y, (int)(text.size() * fontScale * Globals::FONT_WIDTH), (int)(fontScale * Globals::FONT_HEIGHT)};
	this->srcRect = src;
	this->fontScale = fontScale;
	this->canSelect = canSelect;
	this->canClick = canClick;
	this->selected = false;
}

FontSprite::FontSprite(std::string text, SDL_Texture* tex, SDL_Texture* texS, int x, int y, double fontScale, bool canSelect, bool canClick, std::function<void()> onClick) {
	SDL_Rect src = {0, 0, Globals::FONT_WIDTH, Globals::FONT_HEIGHT};

	this->text = text;
	this->texture = tex;
	this->selectedTexture = texS;
	this->destRect = { x, y, (int)(text.size() * fontScale * Globals::FONT_WIDTH), (int)(fontScale * Globals::FONT_HEIGHT)};
	this->srcRect = src;
	this->fontScale = fontScale;
	this->canSelect = canSelect;
	this->canClick = canClick;
	this->selected = false;
	this->onClick = onClick;
}

void FontSprite::CentreHorizontal() {
	double width = text.size() * fontScale * Globals::FONT_WIDTH;

	destRect.x = (int)((Globals::SCREEN_WIDTH / 2) - (width / 2));
	destRect.w = (int)width;
}

void FontSprite::DoClick() {
	// TODO Sound queue here
	if (onClick != NULL) onClick();
}

bool FontSprite::CheckBounds(int x, int y) {
	if (x >= destRect.x) {
		if (x <= destRect.x + destRect.w) {
			if (y >= destRect.y) {
				if (y <= destRect.y + destRect.h) {
					return true;
				}
			}
		}
	}

	return false;
}

void FontSprite::Render(SDL_Renderer* renderer) {
	SDL_Rect drawRect = { destRect.x, destRect.y, (int)(fontScale * Globals::FONT_WIDTH), (int)(fontScale * Globals::FONT_HEIGHT) };

	if (selected) drawRect.x += 50;

	for(char& c : text) {
		srcRect.x = getFontColumn(c) * Globals::FONT_WIDTH;
		srcRect.y = getFontRow(c) * Globals::FONT_HEIGHT;

		SDL_RenderCopy(renderer, selected ? selectedTexture : texture, &srcRect, &drawRect);

		drawRect.x += (int)(fontScale * Globals::FONT_WIDTH);
	}
}

int FontSprite::getFontRow(char c) {
	switch (c) {
		case 'A':
		case 'B':
		case 'C':
		case 'D':
		case 'E':
		case 'F':
		case 'G':
		case 'H':
		case 'I':
		case 'J':
		case 'K':
		case 'L':
		case 'M':
		case 'N':
		case 'O':
		case 'P':
		case 'Q':
		case 'R':
		case 'S':
		case 'T':
		case 'U':
		case 'V':
		case 'W':
		case 'X':
		case 'Y':
		case 'Z':
			return 0;
			break;

		case 'a':
		case 'b':
		case 'c':
		case 'd':
		case 'e':
		case 'f':
		case 'g':
		case 'h':
		case 'i':
		case 'j':
		case 'k':
		case 'l':
		case 'm':
		case 'n':
		case 'o':
		case 'p':
		case 'q':
		case 'r':
		case 's':
		case 't':
		case 'u':
		case 'v':
		case 'w':
		case 'x':
		case 'y':
		case 'z':
			return 1;
			break;

		default:
			return 2;
			break;
	}
}

int FontSprite::getFontColumn(char c) {
	switch(c) {
		case 'A':
		case 'a':
		case '1':
			return 0;
			break;

		case 'B':
		case 'b':
		case '2':
			return 1;
			break;

		case 'C':
		case 'c':
		case '3':
			return 2;
			break;

		case 'D':
		case 'd':
		case '4':
			return 3;
			break;

		case 'E':
		case 'e':
		case '5':
			return 4;
			break;

		case 'F':
		case 'f':
		case '6':
			return 5;
			break;

		case 'G':
		case 'g':
		case '7':
			return 6;
			break;

		case 'H':
		case 'h':
		case '8':
			return 7;
			break;

		case 'I':
		case 'i':
		case '9':
			return 8;
			break;

		case 'J':
		case 'j':
		case '0':
			return 9;
			break;

		case 'K':
		case 'k':
		case '!':
			return 10;
			break;

		case 'L':
		case 'l':
		case '?':
			return 11;
			break;

		case 'M':
		case 'm':
		case ':':
			return 12;
			break;

		case 'N':
		case 'n':
		case ',':
			return 13;
			break;

		case 'O':
		case 'o':
		case '.':
			return 14;
			break;

		case 'P':
		case 'p':
		case ' ':
			return 15;
			break;

		case 'Q':
		case 'q':
			return 16;
			break;

		case 'R':
		case 'r':
			return 17;
			break;

		case 'S':
		case 's':
			return 18;
			break;

		case 'T':
		case 't':
			return 19;
			break;

		case 'U':
		case 'u':
			return 20;
			break;

		case 'V':
		case 'v':
			return 21;
			break;

		case 'W':
		case 'w':
			return 22;
			break;

		case 'X':
		case 'x':
			return 23;
			break;

		case 'Y':
		case 'y':
			return 24;
			break;

		case 'Z':
		case 'z':
		default:
			return 25;
			break;

	}
}
