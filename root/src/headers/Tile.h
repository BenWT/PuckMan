//
// Copyright (c) 2016 by Ben Townshend. All Rights Reserved.
//

#ifndef tile_h
#define tile_h

#include "Sprite.h"
#include "Vector2.h"

enum TileState { Empty, Collider };

class Tile {
public:
	void SetSprite(Sprite sprite) {
		this->sprite = sprite;
	}
	Sprite GetSprite() {
		return sprite;
	}

	// Constructor
	Tile() {
		this->state = Empty;
	}
private:
	Sprite sprite;
	TileState state;
};

#endif
