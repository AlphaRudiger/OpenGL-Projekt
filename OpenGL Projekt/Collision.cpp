#include "Collision.h"

#include <iostream>

std::vector<Collider*> getOverlaps(Collider& c, std::vector<std::vector<Collider>*>& otherColliders) {
	std::vector<Collider*> ol;
	for (std::vector<Collider>* e : otherColliders) {
		for (Collider& oc : *e) {
			if (overlaps(oc, c)) {
				ol.push_back(&oc);
			}
		}
	}
	return ol;
}

bool Collider::canMove(float x, float y, float z, std::vector<std::vector<Collider>*> otherColliders) {
	this->x += x;
	this->y += y;
	this->z += z;
	std::vector<Collider*> o = getOverlaps(*this, otherColliders);
	bool can = true;
	for (Collider* c : o) {
		if (c->type != ColliderType::Dynamic) {
			can = false;
			break;
		}
	}
	this->x -= x;
	this->y -= y;
	this->z -= z;
	return can;
}

Collider::Collider(ColliderType type, float x, float y, float z, float width, float height, float depth) : type(type), x(x), y(y), z(z), width(width), height(height), depth(depth) {
	
}

glm::vec3 Collider::move(float x, float y, float z, std::vector<std::vector<Collider>*> otherColliders) {
	this->x += x;
	this->y += y;
	this->z += z;

	std::vector<Collider*> o = getOverlaps(*this, otherColliders);

	if (o.size()) {
		this->x -= x;
		this->y -= y;
		this->z -= z;
		return {};
	}

	return {x, y, z};
}