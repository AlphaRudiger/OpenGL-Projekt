#pragma once

#include "Utils.h"

#include <unordered_map>

using namespace std;

class World;
class Block;
class Chunk;

struct ChunkPos {
	int x, z;

	inline bool operator==(const ChunkPos& other) const {
		return other.x == x && other.z == z;
	}

	struct HashFunction {
		inline size_t operator()(const ChunkPos& pos) const {
			size_t rowHash = std::hash<int>()(pos.x);
			size_t colHash = std::hash<int>()(pos.z) << 1;
			return rowHash ^ colHash;
		}
	};
};

class ChunkContainer {
	shared_ptr<Chunk> chunk;
	mutex mtx;
	condition_variable cv;
public:

	inline ChunkContainer(Chunk* chunk = nullptr) : chunk(chunk) {

	}

	inline void setChunk(Chunk* chunk) {
		this->chunk = shared_ptr<Chunk>(chunk);
		cv.notify_all();
	}

	inline Chunk* getChunk(bool wait = false) {
		if(!wait || chunk.get() != nullptr) return chunk.get();
		if(chunk.get() == nullptr) {
			unique_lock<mutex> lck(mtx);
			cv.wait(lck);
		}
		return chunk.get();
	}
};

class GeneratePromise {
	World* world;
	ChunkPos pos;
	shared_ptr<ChunkContainer> cc;
	bool onlyMesh;

public:
	inline GeneratePromise() : GeneratePromise(nullptr, { 0, 0 }, nullptr) {

	}

	inline GeneratePromise(World* world, ChunkPos pos, shared_ptr<ChunkContainer> cc, bool onlyMesh = false) : world(world), pos(pos), cc(cc), onlyMesh(onlyMesh) {

	}

	inline Chunk* getChunk() {
		return cc->getChunk();
	}

	void fulfill();
};

class ChunkSupplier {
	//shared_recursive_mutex sm;

	recursive_mutex rm;

	World* world;

	unordered_map<ChunkPos, shared_ptr<ChunkContainer>, ChunkPos::HashFunction> chunks;
public:
	inline unordered_map<ChunkPos, shared_ptr<ChunkContainer>, ChunkPos::HashFunction> copy() {
		lock_guard<recursive_mutex> lck(rm);
		return chunks;
	}

	float time = 0;
	int last = 0;
	inline void monitor(float delta) {
		time += delta;
		if(time <= 3) {
			rm.lock();
			int newA = (int) chunks.size();
			rm.unlock();
			int add = newA - last;
			last = newA;
			cout << "AF: " << add << endl;
			cout << "AV: " << newA / time << endl;
		}
	}

	inline int getChunkCount() {
		lock_guard<recursive_mutex> lck(rm);
		return (int) chunks.size();
	}

	enum LockType {
		None, Shared, Unique
	};

	ChunkSupplier(World*);

	shared_ptr<GeneratePromise> getNextChunkToGenerate();

	template<typename T, typename... Args>
	void forEach(T t, Args... args) {
		forEach<LockType::Shared>(t, args...);
	}

	shared_ptr<ChunkContainer> getSP(ChunkPos pos, bool lock = true) {
		if(lock) rm.lock();
		auto sc = chunks.find({ pos.x, pos.z });
		if(sc != chunks.end()) {
			shared_ptr<ChunkContainer> chunk = sc->second;
			if(lock) rm.unlock();
			return chunk;
		}
		if(lock) rm.unlock();
		return nullptr;
	}

	ChunkContainer* get(ChunkPos pos, bool lock = true) {
		return getSP(pos, lock).get();
	}

	void insert(ChunkPos pos, shared_ptr<ChunkContainer> chunk) {
		synchronized(rm) {
			#ifdef DEBUG
  				ASSERT(get(pos) == nullptr);
			#endif
			chunks.insert({ pos,chunk });
		}
	}

	shared_ptr<ChunkContainer> createChunkContainer(ChunkPos pos) {
		shared_ptr<ChunkContainer> chunk(new ChunkContainer());
		insert(pos, chunk);
		return chunk;
	}

	shared_ptr<GeneratePromise> createGeneratePromise(ChunkPos pos) {
		return shared_ptr<GeneratePromise>(new GeneratePromise(world, pos, createChunkContainer(pos)));
	}

	template<LockType locktype, typename T, typename... Args>
	void forEach(T t, Args... args) {
		using returnType = invoke_result_t<T, Chunk*, Args...>;

		static_assert(is_same_v<returnType, void> || is_same_v<returnType, bool>, "valid types: void, bool");


		synchronized(rm) {
			for(auto& e : chunks) {
				Chunk* chunk = e.second->getChunk();
				if(chunk != nullptr) {
					if constexpr(is_same_v<returnType, bool>) {
						if(t(chunk, args...)) {
							break;
						}
					}
					else {
						t(chunk, args...);
					}
				}
			}
		}
	}

	inline void removeChunk(ChunkPos pos) {
		synchronized(rm) {
			chunks.erase(pos);
		}
	}

	Block* getBlock(int x, int y, int z, bool generateChunk, bool lock = true);
	Chunk* generateChunk(int x, int z, bool updateMesh);
	Chunk* getChunk(int x, int z, bool generateChunk, bool lock = true);

	inline Chunk* getChunkByBlock(int x, int z, bool generateChunk) {
		return getChunk(getChunkNumber(x), getChunkNumber(z), generateChunk);
	}
	inline Chunk* getChunkByLocation(float x, float z, bool generateChunk) {
		return getChunkByBlock(getRealBlockNumber(x), getRealBlockNumber(z), generateChunk);
	}

	~ChunkSupplier();
private:
	shared_ptr<GeneratePromise> gstuff(unordered_map<ChunkPos, shared_ptr<ChunkContainer>, ChunkPos::HashFunction>& chunks, int x, int z);
};