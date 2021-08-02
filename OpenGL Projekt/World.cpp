#include "World.h"

#include "Utils.h"

#include <chrono>
#include <thread>

#include "Player.h"
#include "FrustumCull.h"

World::World(int seed) : generator(this, seed), player(nullptr), gravity(0, 40, 0), chunkSupplier(this), lm(this) {

}

extern Player* stupid;

#include "Profiler.h"
extern Profiler profiler;

void World::draw() {
	vector<Chunk*> chunks;
	PROFILE(profiler, "Sort", {
		chunks.reserve(getChunkSupplier().getChunkCount());
		chunkSupplier.forEach([](Chunk* chunk, vector<Chunk*>* chunks) {
			chunks->push_back(chunk);
		}, &chunks);
		std::sort(chunks.begin(), chunks.end(), [](Chunk* c1, Chunk* c2) {
			return	(abs(abs(c1->x) - abs(getChunkNumber(stupid->position.x))) + abs(abs(c1->z) - abs(getChunkNumber(stupid->position.z)))) >
					(abs(abs(c2->x) - abs(getChunkNumber(stupid->position.x))) + abs(abs(c2->z) - abs(getChunkNumber(stupid->position.z))));
		});
	});

	PROFILE(profiler, "WR", {
		if(frustum) {
			Frustum f(player->cam.combined);
			for(Chunk* chunk : chunks) {
				BoundingBox& boundingBox = chunk->boundingBox;
				if(f.IsBoxVisible(boundingBox.low, boundingBox.high)) {
					chunk->draw();
				}
			}
		}
		else {
			chunkSupplier.forEach([](Chunk* chunk) {
				chunk->draw();
			});
		}
	});
	
}

//void World::removeChunks() {
//	if (!ctdelete.empty()) {
//		for (Chunk* p : ctdelete) {
//			Chunk* nc;
//
//#define gch(ax, az) nc = getChunk(p->x + ax, p->z + az, false); if(nc != nullptr) for(int i = 0; i < ChunkHeight; i++) { DataChunk* dc = &nc->chunks[i]; dc->addUpdateBlocks.clear(); dc->removeUpdateBlocks.clear(); dc->updateBlocks.clear(); }
//			
//			gch(-1, 0);
//			gch(1, 0);
//			gch(0, -1);
//			gch(0, 1);
//
//			delete p;
//		}
//		ctdelete.clear();
//	}
//}

inline void setChunk(Entity* entity, Chunk* chunk) {
	if (entity->chunk != nullptr) {
		int i = 0;
		for (Entity* e : entity->chunk->entitys) {
			if (entity == e) {
				break;
			}
			i++;
		}
		entity->chunk->entitys.erase(entity->chunk->entitys.begin() + i);
	}
	entity->chunk = chunk;
	chunk->entitys.push_back(entity);
}

void World::addEntity(Entity* entity) {
	Chunk* chunk = getChunkByLocation(entity->position.x, entity->position.z, true);
	setChunk(entity, chunk);
}

glm::vec3 World::move(Entity* entity, float x, float y, float z) {

	std::vector<std::vector<Collider>*> chunks = player->getPossibleColliders();

	glm::vec3 diff(0, 0, 0);

	diff += player->collider.move(x, 0, 0, chunks);
	diff += player->collider.move(0, 0, z, chunks);
	diff += player->collider.move(0, y, 0, chunks);

	entity->position += diff;

	Chunk* nc;

	if ((nc = getChunkByLocation(entity->position.x, entity->position.z, true)) != entity->chunk) {
		setChunk(entity, nc);
	}

	return diff;
}

extern GLFWwindow* window;

int maxTicks = 1000 / 20; // 20 tps

static long long ptime = currentTimeMilis();
static long long passedTime;

void World::updateEntitys(float delta) {
	passedTime += currentTimeMilis() - ptime;
	ptime = currentTimeMilis();

	if (passedTime >= maxTicks) {
		int ic = 0;
		passedTime %= maxTicks;
		//for (auto c = chunks.begin(); c != chunks.end(); c++) {
		//	for (int i = 0; i < ChunkHeight; i++) {
		//		DataChunk& dc = c->second->chunks[i];
		//
		//		if (dc.addUpdateBlocks.size()) {
		//			for (Block* a : dc.addUpdateBlocks) {
		//				dc.updateBlocks.push_back(a);
		//			}
		//			dc.addUpdateBlocks.clear();
		//		}
		//
		//		if (dc.removeUpdateBlocks.size()) {
		//			for (Block* r : dc.removeUpdateBlocks) {
		//				int co = 0;
		//				for (Block* cu : dc.updateBlocks) {
		//					if (cu == r) break;
		//					co++;
		//				}
		//				dc.updateBlocks.erase(dc.updateBlocks.begin() + co);
		//			}
		//			dc.removeUpdateBlocks.clear();
		//		}
		//
		//		for (Block* b : dc.updateBlocks) {
		//			b->updateTick();
		//			ic++;
		//		}
		//
		//	}
		//}
	}
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
		chunkSupplier.forEach([](Chunk* chunk) {
			for(int i = 0; i < ChunkHeight; i++) {
				chunk->getDataChunk(i * ChunkSize)->mesh.trim();
				chunk->getDataChunk(i * ChunkSize)->tmesh.trim();
			}
		});
	}
	chunkSupplier.forEach([](Chunk* chunk, World* world, float delta) {
		for(Entity* entity : chunk->entitys) {
			entity->friction = { 30, 0, 30 };
			entity->velocity -= world->gravity * delta;
			entity->update(delta);
		}
	}, this, delta);

	if (migration.size()) {
		migration.clear();
	}
}

World::~World() {
	std::cout << "Delte World..." << std::endl;
}