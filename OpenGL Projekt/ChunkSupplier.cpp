#include "ChunkSupplier.h"

#include "World.h"
#include "Player.h"

void GeneratePromise::fulfill() {
	if(onlyMesh) {
		cc->getChunk()->updateMesh();
	}
	else {
		Chunk* c = new Chunk(pos.x, pos.z);
		world->getWorldGenerator().generateChunk(c, pos.x, pos.z);
		cc->setChunk(c);
	}
}

ChunkSupplier::ChunkSupplier(World* world) : world(world) {

}

bool toggle = false;

shared_ptr<GeneratePromise> ChunkSupplier::gstuff(unordered_map<ChunkPos, shared_ptr<ChunkContainer>, ChunkPos::HashFunction>& chunks, int x, int z) {
	int rx = getChunkNumber((int) world->player->position.x) + x;
	int rz = getChunkNumber((int) world->player->position.z) + z;

	ChunkPos pos = { rx, rz };

	shared_ptr<ChunkContainer> chunk = nullptr;
	auto f = chunks.find(pos);
	if(f != chunks.end()) {
		chunk = f->second;
	}
	if(chunk == nullptr) {
		lock_guard<recursive_mutex> lck(this->rm);
		if((chunk = getSP(pos, false)) == nullptr) {
			return createGeneratePromise(pos);
		}
	}
	else {
		if(chunk->getChunk() != nullptr) {
			if(!chunk->getChunk()->isMeshUpdated()) {

				constexpr int test[] = { 1, 0, -1, 0, 0, 1, 0, -1, 1, 1, -1, -1, 1, -1, -1, 1 };

				for(int i = 0; i < _countof(test); i += 2) {
					ChunkPos npos = { pos.x + test[i], pos.z + test[i + 1] }; 
					ChunkContainer* sc = nullptr; 
					auto ff = chunks.find(npos); 
					if(ff != chunks.end()) {
						sc = ff->second.get(); 
					}
					if(sc == nullptr) {
						if((sc = this->get(npos)) == nullptr) {
							return createGeneratePromise(npos); 
						}
					}
					if(sc->getChunk() == nullptr) {
						return nullptr;
					}
				}

				shared_ptr<GeneratePromise> gp = shared_ptr<GeneratePromise>(new GeneratePromise(world, pos, chunk, true));
				return gp;
			}
		}
	}
	return nullptr;
	//Chunk* c = getChunk(rx, rz, true);
	//if(!c->isMeshUpdated()) {
	//	//std::cout << "updated Missing mesh" << c->x << " " << c->z << std::endl;
	//	c->firstUpdateTick();
	//	c->updateMesh();
	//	return;
	//}
}

shared_ptr<GeneratePromise> ChunkSupplier::getNextChunkToGenerate() {
	unordered_map<ChunkPos, shared_ptr<ChunkContainer>, ChunkPos::HashFunction> chunks = copy();
	toggle = !toggle;

	GeneratePromise* p = nullptr;
	for(int i = 1; i < RenderDistance + 1; i++) {
		for(int x = -i; x < i; x++) {
			for(int z = -i; z < i; z++) {
				if(x > -i && x < i - 1 && z > -i && z < i - 1) {
					z = i - 2;
					continue;
				}
				shared_ptr<GeneratePromise> r = gstuff(chunks, x, z);
				if(r != nullptr) {
					return r;
				}
			}
		}
	}
	//for(int x = 0; x < RenderDistance * 2; x++) {
	//	int ix = toggle ? RenderDistance * 2 - 1 - x : x;
	//	//ix = RenderDistance - 1 - abs(ix);
	//	ix *= mapN(ix);
	//	for(int z = 0; z < RenderDistance * 2; z++) {
	//		shared_ptr<GeneratePromise> r = gstuff(ix, z);
	//		if(r != nullptr) {
	//			return r;
	//		}
	//	}
	//}
	return nullptr;
}

Chunk* ChunkSupplier::getChunk(int x, int z, bool sgenerateChunk, bool lock) {
	if(lock) rm.lock();
	ChunkContainer* chunk;
	chunk = get({ x, z });

	if(chunk == nullptr) {
		if(sgenerateChunk) {
			Chunk* c = this->generateChunk(x, z, false);
			if(lock) rm.unlock();
			return c;
		}
		if(lock) rm.unlock();
		return nullptr;
	}
	if(lock) rm.unlock();
	return chunk->getChunk(sgenerateChunk);
}

Chunk* ChunkSupplier::generateChunk(int x, int z, bool updateMesh) {
	
	shared_ptr<GeneratePromise> promise = createGeneratePromise({ x, z });
	promise->fulfill();
	Chunk* c = promise->getChunk();
	if(updateMesh) {
		c->firstUpdateTick();
		TestSpeed(c->updateMesh());
	}
	return c;
}

Block* ChunkSupplier::getBlock(int x, int y, int z, bool generateChunk, bool lock) {
	if(y < 0 || y > ChunkSize * ChunkHeight) return nullptr;
	int cx = getChunkNumber(x);
	int cz = getChunkNumber(z);

	Chunk* chunk = getChunk(cx, cz, generateChunk, lock);

	if(chunk == nullptr) return nullptr;

	int rx = x - cx * ChunkSize;
	int rz = z - cz * ChunkSize;

	return chunk->getBlock(rx, y, rz);
}

ChunkSupplier::~ChunkSupplier() {
	synchronized(rm) {
		chunks.clear();
	}
}