//
// Copyright (c) 2016 by Ben Townshend. All Rights Reserved.
//

#pragma once

#include "Globals.h"
#include "Sprite.h"
#include "Vector2.h"

class Tile {
public:
	void SetSprite(Sprite);
	Sprite GetSprite();
	void SetState(int);
	int GetState();

	Vector2* GetPosition();
	Vector2* GetTexturePosition();
	double GetPositionX();
	double GetPositionY();
	double GetTextureX();
	double GetTextureY();

	Tile();
	Tile(int);
	Tile(int, int);
private:
	Sprite sprite;
	int state;
	Vector2* position;
	Vector2* texturePosition;
};
