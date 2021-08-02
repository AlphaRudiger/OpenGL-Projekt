#include "MinecraftTextures.h"

#include "Block.h"
#include "Chunk.h"

TR nanTexture = { nullptr, 0, 0, 16, 16 };
#include <GL/glew.h>
TextureContainer::TextureContainer(TR& texture) : TextureContainer(texture, texture, texture, texture, texture, texture) {
	
}

TextureContainer::TextureContainer(TR& front, TR& back, TR& top, TR& bottom, TR& left, TR& right) : front(front), back(back), top(top), bottom(bottom), left(left), right(right) {

}

#define DefaultMaterialValues nanTexture, true

Material Material::AIR(DefaultMaterialValues, RenderType::None);
Material Material::DIRT(DefaultMaterialValues);
Material Material::GRASS_BLOCK(DefaultMaterialValues);
Material Material::STONE(DefaultMaterialValues);
Material Material::WOOD_LOG(DefaultMaterialValues);
Material Material::GRASS(DefaultMaterialValues);

void loadTextures(TextureAtlas& atlas) {
    TR dirt = atlas.map["Dirt"];
    TR grass_side = atlas.map["Grass_Side"];
    TR grass_top = atlas.map["Grass_Top"];
    TR stone = atlas.map["Stone"];
    TR wood_log_side = atlas.map["Wood_Log_Side"];
    TR wood_log_top = atlas.map["Wood_Log_Top"];
	TR grass = atlas.map["Grass"];
	Material::DIRT.set(atlas.map["Dirt"], false, RenderType::BlockShape, [](Block* block) {

        if (block->getBlock(block->x, block->y + 1, block->z)->material != &Material::AIR) {
            block->removeUpdateList();
            return;
        }
        for (int x = 0; x < 3; x++) {
            for (int y = 0; y < 3; y++) {
                for (int z = 0; z < 3; z++) {
                    if (x == 1 && z == 1) continue;
					Block* gblock = block->getBlock(block->x + x - 1, block->y + y - 1, block->z + z - 1);
					if(gblock != nullptr) {
						if(gblock->material == &Material::GRASS_BLOCK) {
							block->addUpdateList();
						}
					}
                }
            }
        }

    }, [](Block* block) {
        if (rand() % 1000 == 1) {
            block->material = &Material::GRASS_BLOCK;
            block->inUpdateList = false;
            block->chunk->removeUpdateBlocks.push_back(block);
            block->updateAroundThisBlock();
            block->chunk->meshNeedsUpdate();
        }
    });
    Material::GRASS_BLOCK.set({ grass_side, grass_side, grass_top, dirt, grass_side, grass_side }, false, RenderType::BlockShape, [](Block* block) {
        if (block->getBlock(block->x, block->y + 1, block->z)->material != &Material::AIR) {
            block->addUpdateList();
            return;
        }
        block->removeUpdateList();
    }, [](Block* block) {
        if (rand() % 100 == 1) {
            block->material = &Material::DIRT;
            block->removeUpdateList();
            block->updateAroundThisBlock();
            block->chunk->meshNeedsUpdate();
        }
    });
    Material::STONE.set({ stone }, false);
	Material::WOOD_LOG.set({ wood_log_side, wood_log_side, wood_log_top, wood_log_top, wood_log_side, wood_log_side });
	Material::GRASS.set({ grass }, true, RenderType::XShape);
}