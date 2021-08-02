#include "WorldGenerator.h"

#include "Chunk.h"
#include "World.h"



WorldGenerator::WorldGenerator(World* world, int seed) : world(world), seed(seed), noise(seed), caveGenerator(seed) {
	noise.SetNoiseType(FastNoiseLite::NoiseType::NoiseType_Perlin);
	caveGenerator.SetNoiseType(FastNoiseLite::NoiseType::NoiseType_Perlin);
}

#define idkheight 120
#define diff 50

int getHeigth(WorldGenerator* wg, int x, int z) {
	return idkheight + diff;
	//return (int) ((wg->noise.GetNoise(x * 3.f, z * 3.f) + 1) / 2 * diff + idkheight);
}

void WorldGenerator::generateChunk(Chunk* chunk, int xx, int zz) {
	srand(xx << 16 ^ zz);

	chunk->x = xx;
	chunk->z = zz;
	chunk->world = world;

	int ox = ChunkSize * xx;
	int oz = ChunkSize * zz;
	
	int trees = rand() % 3;
	for (int x = 0; x < ChunkSize; x++) {
		for (int z = 0; z < ChunkSize; z++) {
			int height = getHeigth(this, ox + x, oz + z);
			for (int y = 0; y < height; y++) {
				if (abs(caveGenerator.GetNoise((x + ox) * 2.f, y *2.f, (z + oz) * 2.f)) > 0.5f) continue;
				Block* b = chunk->getBlock(x, y, z);
				if (b == nullptr) {
					continue;
				}
				//if(y == height - 1) {
				//	b->setMaterial(Material::GRASS);
				//}
				else
				if (y == height - 1) {
					b->setMaterial(Material::GRASS_BLOCK);
				}
				else if(y > height - 5) {
					b->setMaterial(Material::DIRT);
				}
				else {
					b->setMaterial(Material::STONE);
				}
			}
		}
	}

	for (int i = 0; i < trees; i++) {
		int x = rand() % ChunkSize;
		int z = rand() % ChunkSize;
	
		int landHeight = (int)((noise.GetNoise((x + ox) * 3.0f, (z + oz) * 3.0f) + 1) / 2 * 20 + idkheight);
	
		int height = rand() % 4 + 4;

		Block* b = chunk->getBlock(x, landHeight - 1, z);

		if (b != nullptr) {
			if (b->material != &Material::GRASS_BLOCK) {
				continue;
			}
			else {
				b->setMaterial(Material::DIRT);
			}


			for (int j = 0; j < height; j++) {
				Block* b = chunk->getBlock(x, landHeight + j, z);
				if (b == nullptr) {
					continue;
				}
				b->setMaterial(Material::WOOD_LOG);
			}
		}
	}
}