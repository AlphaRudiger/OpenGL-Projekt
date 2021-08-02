#include "Entity.h"
#include "Utils.h"
#include "World.h"

std::vector<std::vector<Collider>*> Entity::getPossibleColliders() {
	std::vector<std::vector<Collider>*> colliders;
	for (DataChunk* c : getDataChunks()) {
		colliders.push_back(&c->colliders);
	}
	return colliders;
}

std::vector<DataChunk*> Entity::getDataChunks() {
	bool adc = true;
	Collider& coll = collider;
	std::vector<DataChunk*> chunks;
	Chunk* nc;
	DataChunk* cc;
#define add(dx, dy, dz)	nc = world->getChunkByBlock(getRealBlockNumber(dx), getRealBlockNumber(dz), false); if(nc != nullptr) {cc = nc->getDataChunk(getRealBlockNumber(dy)); if(cc != nullptr) {adc = true;\
						for(DataChunk* dc : chunks) if(dc == cc) { adc = false; break; } if(adc) chunks.push_back(cc);}}
	const float e = 0.5f;

	add(coll.x - e, coll.y - e, coll.z - e);
	add(coll.x + coll.width + e, coll.y - e, coll.z - e);
	add(coll.x - e, coll.y + coll.height + e, coll.z - e);
	add(coll.x - e, coll.y - e, coll.z + coll.depth + e);
	add(coll.x + coll.width + e, coll.y + coll.height + e, coll.z - e);
	add(coll.x + coll.width + e, coll.y - e, coll.z + coll.depth + e);
	add(coll.x + coll.width + e, coll.y + coll.height + e, coll.z + coll.depth + e);
	add(coll.x - e, coll.y + coll.height + e, coll.z + coll.depth + e);

	return chunks;
}

bool Entity::isOnGround() {
	std::vector<std::vector<Collider>*> colliders;
	for (DataChunk* c : getDataChunks()) {
		colliders.push_back(&c->colliders);
	}
	return !collider.canMove(0, -0.01f, 0, colliders);
}