#pragma once

#include <unordered_map>
#include <vector>

#include "Chunk.h"
#include "WorldGenerator.h"
#include "Utils.h"
#include "Entity.h"
#include "ChunkSupplier.h"
#include "Light.h"

class Player;

struct Migration {
	Entity* entity;
	Chunk* target;
};

class World {
public:
	LightManager lm;
	ChunkSupplier chunkSupplier;
	std::vector<Migration> migration;
	glm::vec3 gravity;
public:
	WorldGenerator generator;
	Player* player;

	World(int seed);

	void updateEntitys(float delta);
	glm::vec3 move(Entity* entity, float x, float y, float z);
	//void removeChunks();
	void addEntity(Entity* entity);
	void draw();

	inline WorldGenerator getWorldGenerator() {
		return generator;
	}

	inline Chunk* generateChunk(int x, int z, bool updateMesh) {
		return chunkSupplier.generateChunk(x, z, updateMesh);
	}
	inline Block* getBlock(int x, int y, int z, bool generateChunk) {
		return chunkSupplier.getBlock(x, y, z, generateChunk);
	}
	inline Chunk* getChunk(int x, int z, bool generateChunk) {
		return chunkSupplier.getChunk(x, z, generateChunk);
	}
	inline Chunk* getChunkByBlock(int x, int z, bool generateChunk) {
		return getChunk(getChunkNumber(x), getChunkNumber(z), generateChunk);
	}
	inline Chunk* getChunkByLocation(float x, float z, bool generateChunk) {
		return getChunkByBlock(getRealBlockNumber(x), getRealBlockNumber(z), generateChunk);
	}

	inline ChunkSupplier& getChunkSupplier() {
		return chunkSupplier;
	}

	~World();

};