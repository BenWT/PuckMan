//
// Copyright (c) 2016 by Ben Townshend. All Rights Reserved.
//

#include <cmath>

#ifndef vector2_H
#define vector2_H

class Vector2 {
public:
	double x;
	double y;
	Vector2() {
		x = 0;
		y = 0;
	}
	Vector2(double _x, double _y) {
		x = _x;
		y = _y;
	}
	double Magnitude() {
		return sqrt((x * x) + (y * y));
	}
	void Move(double _x, double _y) {
		x += _x;
		y += _y;
	}
	void Set(double _x, double _y) {
		x = _x;
		y = _y;
	}
};

#endif
