#pragma once

#include <vector>

#include "Collision.h"
#include "Utils.h"

class Chunk;
class DataChunk;
class World;

class Entity {
public:
	Collider collider;
	glm::vec3 position;
	glm::vec3 velocity;
	Vector3G friction;
	World* world;
	Chunk* chunk;
	bool triesToMove;

public:
	inline Entity(World* world, ColliderType type, float x, float y, float z, float width, float height, float depth) : world(world), collider(type, x, y, z, width, height, depth), position(x, y, z), velocity(0, 0, 0), friction(0, 0, 0), chunk(nullptr), triesToMove(false) {

	}
	std::vector<std::vector<Collider>*> getPossibleColliders();
	std::vector<DataChunk*> getDataChunks();
	virtual void update(float delta) = 0;
	bool isOnGround();
	

};
