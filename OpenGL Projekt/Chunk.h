#pragma once

#include "Utils.h"
#include "Mesh.h"
#include "Block.h"
#include "Collision.h"
#include "Entity.h"
#include "BoundingBox.h"
#include "Light.h"

class Chunk;
class World;

class DataChunk {
public:
	Block blocks[ChunkSize][ChunkSize][ChunkSize];
	int y;
	Chunk* chunk;
	Mesh mesh, tmesh;
	std::vector<Collider> colliders;
	std::vector<Block*> updateBlocks;
	std::vector<Block*> addUpdateBlocks;
	std::vector<Block*> removeUpdateBlocks;
	std::vector<LightNode> lights;
private:
	bool volatile updatedMesh = false;
public:
	DataChunk();
	DataChunk(Chunk* chunk, int y);
	void addLight(LightNode light);
	void updateMesh();
	void updateMeshFromChangedBlock(int x, int y, int z, bool directUpdate = false);
	void draw();

	inline void meshNeedsUpdate() {
		updatedMesh = false;
	}

	inline bool isMeshUpdated() {
		return updatedMesh;
	}
};

class Chunk {
public:
	BoundingBox boundingBox;
	std::vector<Entity*> entitys;
	DataChunk chunks[ChunkHeight];
	World* world;
	int x, z;

public:
	Chunk();
	Chunk(int x, int z);
	
	Block* getBlock(int x, int y, int z);
	void updateMesh();
	void updateMeshFromChangedBlock(int x, int y, int z);
	void firstUpdateTick();
	void updateTick();
	void draw();
	
	inline DataChunk* getDataChunk(int y) {
		int c = y / ChunkSize - (y < 0 ? 1 : 0);
		if ((c < 0) || (c > ChunkHeight - 1)) {
#ifdef DEBUG
			std::cout << "Return nullptr for DataChunk " << y << std::endl;
#endif
			return nullptr;
		}
		return &chunks[c];
	}

	inline bool isMeshUpdated() {
		for (int i = 0; i < ChunkHeight; i++) {
			if (!chunks[i].isMeshUpdated()) {
				return false;
			}
		}
		return true;
	}

	~Chunk();
};