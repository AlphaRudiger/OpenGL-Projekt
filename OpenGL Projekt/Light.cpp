#include "Light.h"

#include "Block.h"
#include "Chunk.h"
#include "World.h"
#include "General.h"

struct LightBlock {
	Material* mat;
	LightLevel light = 0;
};

struct LightChunk {
	DataChunk* chunk = nullptr;
	LightBlock blocks[ChunkSize][ChunkSize][ChunkSize];
public:
	int x = 0, y = 0, z = 0;

	LightChunk() {

	}
};

class LightCalculation {
public:
	unordered_map<ChunkPos, Chunk*, ChunkPos::HashFunction> oc;
	unordered_map<ChunkPos, shared_ptr<LightChunk[]>, ChunkPos::HashFunction> nc;
	
	LightCalculation(DataChunk* chunk) {
		int cx = chunk->chunk->x;
		int cz = chunk->chunk->z;
		constexpr int idk[] = {1, -1, 1, 0, 1, 1, 0, -1, 0, 1, -1, -1, -1, 0, -1, 1, 0, 0};

		for(int i = 0; i < _countof(idk); i += 2) {
			ChunkPos pos = { cx + idk[i], cz + idk[i + 1] }; 
			oc.insert({ {idk[i], idk[i + 1] }, chunk->chunk->world->getChunk(pos.x, pos.z, false) });
		}

		for(int i = 0; i < _countof(idk); i += 2) {
			ChunkPos pos = { idk[i], idk[i + 1] };
			shared_ptr<LightChunk[]> l = shared_ptr<LightChunk[]>(new LightChunk[3]);
			nc.insert({ pos, l });
			Chunk* c = oc[pos];
			for(int j = 0; j < 3; j++) {
				int cy = chunk->y + j - 1;
				if(cy >= 0 && cy < ChunkHeight) {
					DataChunk& cc = c->chunks[cy];
					l[j].chunk = &cc;
					l[j].x = pos.x;
					l[j].y = j - 1;
					l[j].z = pos.z;
					for(int x = 0; x < ChunkSize; x++) {
						for(int y = 0; y < ChunkSize; y++) {
							for(int z = 0; z < ChunkSize; z++) {
								Block& b = cc.blocks[x][y][z];
								LightBlock& lb = l[j].blocks[x][y][z];
								lb.mat = b.material;
							}
						}
					}

				}
			}
		}
	}

	LightBlock* getBlock(int x, int y, int z, LightChunk& chunk) {
		x += chunk.x * ChunkSize;
		y += chunk.y * ChunkSize;
		z += chunk.z * ChunkSize;
		LightChunk* lchunk = getChunk(getChunkNumber(x), getChunkNumber(y), getChunkNumber(z));
		if(lchunk == nullptr) return nullptr;
		//cout << "return block for " << x << " " << y << " " << z << " " << lchunk->x << " " << lchunk->y << " " << lchunk->z << " " << x - lchunk->x * ChunkSize << " " << y - lchunk->y * ChunkSize << " " << z - lchunk->z * ChunkSize << endl;
		return &lchunk->blocks[x - lchunk->x * ChunkSize][y - lchunk->y * ChunkSize][z - lchunk->z * ChunkSize];
	}
	LightBlock* getBlock(Vector3G vec, LightChunk& chunk) {
		return getBlock((int)vec.x, (int)vec.y, (int)vec.z, chunk);
	}

	inline LightChunk* getChunk(int x, int y, int z) {
		if(x < -1 || x > 1 || y < -1 || y > 1 || z < -1 || z > 1) return nullptr;
		ChunkPos pos = { x, z };
		auto& lca = nc[pos];

		return &lca[y + 1];
	}

};

void al(LightCalculation& lc, LightChunk& chunk) {
	vector<LightNode> light = chunk.chunk->lights;
	light.reserve(min((int) light.size() * (int) pow(MAX_LIGHT_LEVEL, 2), (int) pow(ChunkSize, 3)));

	for(int i = 0; i < light.size(); i++) {
		LightNode nn = light[i];
		LightBlock* clb = lc.getBlock(nn.pos, chunk);
		if(clb->light.getIntensity() < nn.intensity) {
			Vector3IG idk1 = Vector3IG(nn.pos.x, nn.pos.y, nn.pos.z) + Vector3IG(chunk.x, chunk.y, chunk.z) * ChunkSize;
			if(dist(idk1, Vector3IG(0, 0, 0)) >= nn.intensity) continue;
			clb->light = nn.intensity;
			nn.intensity--;
			//cout << "light it up: " << nn.pos.x << " " << nn.pos.y << " " << nn.pos.z << " " << nn.intensity << endl;
			if(nn.intensity > 0) {
				for(BlockFace f : blockFaces) {
					Vector3G pos = nn.pos + getNormal(f);
					LightBlock* nlb = lc.getBlock(pos, chunk);
					if(nlb != nullptr) {
						if(nlb->mat->isTransparent()) {
							if(nlb->light.getIntensity() < nn.intensity) {
								if(nn.intensity == 1) {
									nlb->light = nn.intensity;
								}
								else {
									light.push_back(LightNode(pos, nn.color, nn.intensity));
								}
							}
						}
					}
				}
			}
		}
	}
}
#include <unordered_set>
void LightManager::applyLight(DataChunk* chunk) {
	LightCalculation lc = chunk;
	LightChunk& lchunk = *lc.getChunk(0, 0, 0);
	constexpr int idk[] = { 1, -1, 1, 0, 1, 1, 0, -1, 0, 1, -1, -1, -1, 0, -1, 1, 0, 0 };
	for(int i = 0; i < _countof(idk); i += 2) {
		for(int j = 0; j < 3; j++) {
			LightChunk& cc = *lc.getChunk(idk[i], j - 1, idk[i + 1]);
			//std::unordered_set<LightBlock*> set;
			//set.reserve(pow(ChunkSize, 3));
			//for(int x = 0; x < ChunkSize; x++) {
			//	for(int y = 0; y < ChunkSize; y++) {
			//		for(int z = 0; z < ChunkSize; z++) {
			//			ASSERT(lc.getBlock(x, y, z, cc) != nullptr);
			//			set.insert(lc.getBlock(0, y, z, cc));
			//		}
			//	}
			//}
			//ASSERT(set.size() == pow(ChunkSize, 3));
			al(lc, cc);
		}
	}

	//for(int i = 0; i < _countof(idk); i += 2) {
	//	for(int j = 0; j < 3; j++) {
	//		LightChunk& cc = *lc.getChunk(idk[i], j - 1, idk[i + 1]);
	//		for(int x = 0; x < ChunkSize; x++) {
	//			for(int y = 0; y < ChunkSize; y++) {
	//				for(int z = 0; z < ChunkSize; z++) {
	//					cc.chunk->blocks[x][y][z].light = cc.blocks[x][y][z].light;
	//				}
	//			}
	//		}
	//		cc.chunk->meshNeedsUpdate();
	//		cc.chunk->updateMesh();
	//	}
	//}
	for(int x = 0; x < ChunkSize; x++) {
		for(int y = 0; y < ChunkSize; y++) {
			for(int z = 0; z < ChunkSize; z++) {
				lchunk.chunk->blocks[x][y][z].light = lchunk.blocks[x][y][z].light;
			}
		}
	}
}