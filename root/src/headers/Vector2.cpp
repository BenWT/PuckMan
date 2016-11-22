//
// Copyright (c) 2016 by Ben Townshend. All Rights Reserved.
//

#include "Vector2.h"

double Vector2::Magnitude() {
    return sqrt((x * x) + (y * y));
}
void Vector2::Move(double x, double y) {
    this->x += x;
    this->y += y;
}
void Vector2::Set(double x, double y) {
    this->x = x;
    this->y = y;
}

Vector2::Vector2() {
    this->x = 0;
    this->y = 0;
}
Vector2::Vector2(double x, double y) {
    this->x = x;
    this->y = y;
}
