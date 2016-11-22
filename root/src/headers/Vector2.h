//
// Copyright (c) 2016 by Ben Townshend. All Rights Reserved.
//

#pragma once

#include <cmath>

class Vector2 {
public:
	double x;
	double y;
	double Magnitude();
	void Move(double, double);
	void Set(double, double);

	Vector2();
	Vector2(double, double);
};
