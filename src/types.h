#pragma once

#include <math.h>

struct Point {
	int x;
	int y;
};

struct Vector {
	float x;
	float y;
};

struct Segment {
	Vector origin;
	Vector ending;
};

struct Rect {
	float left;
	float top;
	float right;
	float bottom;
};

bool operator== (const Rect& l, const Rect& r) {
	return l.left == r.left && l.top == r.top && l.right == r.right &&
		l.bottom == r.bottom;
}

float sqrDist(Vector a, Vector b) {
	return a.x * b.x + a.y * b.y;
}

float length(Vector v) {
	return sqrtf(v.x * v.x + v.y * v.y);
}

Vector normalize(Vector v) {
	const float len = length(v);
	return { v.x / len, v.y / len };
}

float dotProduct(Vector a, Vector b) {
	return a.x * b.x + a.y * b.y;
}
