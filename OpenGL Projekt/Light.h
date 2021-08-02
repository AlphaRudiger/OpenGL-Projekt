#pragma once

#include "Utils.h"
#include "Color.h"

#define MAX_LIGHT_LEVEL 16

class LightNode {
public:
	Color color;
	Vector3G pos;
	int intensity;
public:
	inline LightNode(Vector3G pos, Color color, int intensity) : color(color), pos(pos), intensity(intensity) {

	}
};

class LightLevel {
	u8 intensity;

public:
	inline LightLevel() : LightLevel(0) {

	}

	inline LightLevel(u8 intensity) : intensity(intensity) {

	}

	inline Vector3G getColor() {
		float idk = (float) intensity / MAX_LIGHT_LEVEL;
		return { idk, idk, idk };
	}

	inline u8 getIntensity() {
		return intensity;
	}
};

class World;
class Chunk;
class DataChunk;

class LightManager {
	World* world;
	
public:
	LightManager(World* world) : world(world) {

	}

	void applyLight(Chunk* chunk);
	void applyLight(DataChunk* chunk);

};