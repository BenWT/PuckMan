//
// Copyright (c) 2016 by Ben Townshend. All Rights Reserved.
//

#include "FontSprite.h"
#include "Globals.h"

void FontSprite::Render(SDL_Renderer* renderer) {
	for(char& c : text) {
		srcRect.x = getFontColumn(c) * Globals::FONT_SIZE;
		srcRect.y = getFontRow(c) * Globals::FONT_SIZE;

	    SDL_RenderCopy(renderer, texture, &srcRect, &destRect);
		destRect.x += destRect.w;
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
	}
}

int FontSprite::getFontColumn(char c) {
	switch(c) {
		case 'A':
		case 'a':
			return 0;
			break;

		case 'B':
		case 'b':
			return 1;
			break;

		case 'C':
		case 'c':
			return 2;
			break;

		case 'D':
		case 'd':
			return 3;
			break;

		case 'E':
		case 'e':
			return 4;
			break;

		case 'F':
		case 'f':
			return 5;
			break;

		case 'G':
		case 'g':
			return 6;
			break;

		case 'H':
		case 'h':
			return 7;
			break;

		case 'I':
		case 'i':
			return 8;
			break;

		case 'J':
		case 'j':
			return 9;
			break;

		case 'K':
		case 'k':
			return 10;
			break;

		case 'L':
		case 'l':
			return 11;
			break;

		case 'M':
		case 'm':
			return 12;
			break;

		case 'N':
		case 'n':
			return 13;
			break;

		case 'O':
		case 'o':
			return 14;
			break;

		case 'P':
		case 'p':
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
			return 25;
			break;

	}
}
