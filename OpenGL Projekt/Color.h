#pragma once

struct Color {
	static Color RED, GREEN, BLUE, WHITE, BLACK;

	float r, g, b, a;

	inline Color(float r, float g, float b, float a = 1) : r(r), g(g), b(b), a(a) {

	}
};
