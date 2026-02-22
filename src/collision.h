#pragma once

#include "types.h"

inline bool inside(Vector v, Rect r) {
	return v.x > r.left && v.x <= r.right && v.y > r.top && v.y <= r.bottom;
}

inline float reflectSymmetrical(float v, float center) {
	return 2.0f * center - v;
}

inline bool reflectOnceInside(Segment& l, Rect r) {
	if (l.ending.x < r.left && l.origin.x >= r.left) {
		l.origin.x = reflectSymmetrical(l.origin.x, r.left);
		l.ending.x = reflectSymmetrical(l.ending.x, r.left);
		return true;
	}

	if (l.ending.x > r.right && l.origin.x <= r.right) {
		l.origin.x = reflectSymmetrical(l.origin.x, r.right);
		l.ending.x = reflectSymmetrical(l.ending.x, r.right);
		return true;
	}

	if (l.ending.y < r.top && l.origin.y >= r.top) {
		l.origin.y = reflectSymmetrical(l.origin.y, r.top);
		l.ending.y = reflectSymmetrical(l.ending.y, r.top);
		return true;
	}

	if (l.ending.y > r.bottom && l.origin.y <= r.bottom) {
		l.origin.y = reflectSymmetrical(l.origin.y, r.bottom);
		l.ending.y = reflectSymmetrical(l.ending.y, r.bottom);
		return true;
	}

	return false;
}

// This assumes origin is inside, ending is outside
inline bool reflectInside(Segment& l, Rect r) {
	bool reflected = false;
	while (reflectOnceInside(l, r)) {
		reflected = true;
	}
	return reflected;
}

// This assumes origin is outside, enging is inside
inline bool reflectOutside(Segment& l, Rect r, Vector& p) {
	// Left
	if (l.ending.x > r.left && l.origin.x <= r.left &&
		l.ending.y > r.top && l.ending.y <= r.bottom) {
		p = { r.left, (r.left - l.origin.x) * (l.ending.y - l.origin.y) / (l.ending.x - l.origin.x) + l.origin.y };
		l.origin.x = reflectSymmetrical(l.origin.x, r.left);
		l.ending.x = reflectSymmetrical(l.ending.x, r.left);
		return true;
	}

	// Right
	if (l.ending.x < r.right && l.origin.x >= r.right &&
		l.ending.y > r.top && l.ending.y <= r.bottom) {
		p = { r.right, (r.right - l.origin.x) * (l.ending.y - l.origin.y) / (l.ending.x - l.origin.x) + l.origin.y };
		l.origin.x = reflectSymmetrical(l.origin.x, r.right);
		l.ending.x = reflectSymmetrical(l.ending.x, r.right);
		return true;
	}

	// Top
	if (l.ending.y > r.top && l.origin.y <= r.top &&
		l.ending.x > r.left && l.ending.x <= r.right) {
		p = { (r.top - l.origin.y) * (l.ending.x - l.origin.x) / (l.ending.y - l.origin.y) + l.origin.x, r.top };
		l.origin.y = reflectSymmetrical(l.origin.y, r.top);
		l.ending.y = reflectSymmetrical(l.ending.y, r.top);
		return true;
	}

	// Bottom
	if (l.ending.y < r.bottom && l.origin.y >= r.bottom &&
		l.ending.x > r.left && l.ending.x <= r.right) {
		p = { (r.bottom - l.origin.y) * (l.ending.x - l.origin.x) / (l.ending.y - l.origin.y) + l.origin.x, r.bottom };
		l.origin.y = reflectSymmetrical(l.origin.y, r.bottom);
		l.ending.y = reflectSymmetrical(l.ending.y, r.bottom);
		return true;
	}

	return false;
}

inline bool reflect(Segment& l, Rect r) {
	const bool originInside = inside(l.origin, r);
	const bool endingInside = inside(l.ending, r);
	if (originInside && endingInside) {
		return false;
	}
	else if (originInside && !endingInside) {
		return reflectInside(l, r);
	}
	else if (!originInside && endingInside) {
		Vector p;
		return reflectOutside(l, r, p);
	}
	else {
		return false;
	}
}

inline Vector roundReflectionNormal(Vector point, Rect rect) {
	// -1 for the left side, 0 for the center, 1 for the right side
	float factor = 2.0f * (point.x - rect.left) / (rect.right - rect.left) - 1.0f;

	// Lerp between {0, -1} and {1, 0}
	const Vector result{
		0.0f + (1.0f - 0.0f) * factor,
		-1.0f + (0.0f + 1.0f) * factor
	};

	return normalize(result);
}

inline bool reflectRound(Segment& s, Rect r) {
	const bool originInside = inside(s.origin, r);
	const bool endingInside = inside(s.ending, r);
	if (originInside || !endingInside) {
		return false;
	}

	Vector p;
	Segment l = s;
	if (!reflectOutside(l, r, p)) {
		return false;
	}

	// General reflection: r = d - 2 (d dot n) n
	const Vector n = roundReflectionNormal(p, r);
	const Vector vs = { s.ending.x - s.origin.x, s.ending.y - s.origin.y };
	const float len = length(vs);
	const Vector d = { vs.x / len, vs.y / len };
	const float dot = dotProduct(d, n);
	const Vector refl = {
		d.x - 2.0f * dot * n.x,
		d.y - 2.0f * dot * n.y
	};

	const float originScale = length({ p.x - s.origin.x, p.y - s.origin.y });
	const float endingScale = length({ s.ending.x - p.x, s.ending.y - p.y });
	s = {
		.origin = { p.x - refl.x * originScale, p.y - refl.y * originScale },
		.ending = { p.x + refl.x * endingScale, p.y + refl.y * endingScale }
	};

	return true;
}
