#pragma once

#include <FastNoiseLite.h>

#include "Chunk.h"

class World;

class WorldGenerator {
public:
	FastNoiseLite noise;
	FastNoiseLite caveGenerator;
	World* world;
	int seed;
public:
	WorldGenerator(World* world, int seed);

	void generateChunk(Chunk* chunk, int x, int z);
	
};
