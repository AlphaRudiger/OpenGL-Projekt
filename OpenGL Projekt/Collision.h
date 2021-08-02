#pragma once

#include <vector>

#include <glm.hpp>

enum class ColliderType {
	Dynamic, Static, Kinematic
};

struct Collider {
	ColliderType type;

	float x, y, z;
	float width, height, depth;

	Collider(ColliderType type, float x, float y, float z, float width, float height, float depth);
	
	glm::vec3 move(float x, float y, float z, std::vector<std::vector<Collider>*> otherColliders);
	bool canMove(float x, float y, float z, std::vector<std::vector<Collider>*> otherColliders);
};



inline bool overlaps(Collider& c1, Collider& c2) {
	return(c1.x < c2.x + c2.width && c1.x + c1.width > c2.x) && (c1.y < c2.y + c2.height && c1.y + c1.height > c2.y) && (c1.z < c2.z + c2.depth && c1.z + c1.depth > c2.z);
}
