#pragma once

#include <glm.hpp>
#include "Utils.h"

class BoundingBox {
public:
	Vector3G low, high;

	BoundingBox(Vector3G v1, Vector3G v2) : low(min(v1.x, v2.x), min(v1.y, v2.y), min(v1.z, v2.z)), high(max(v1.x, v2.x), max(v1.y, v2.y), max(v1.z, v2.z)) {
		
	}

	inline float getWidth() {
		return high.x - low.x;
	}

	inline float getHeight() {
		return high.y - low.y;
	}

	inline float getDepth() {
		return high.z - low.z;
	}
};