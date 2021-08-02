#include "Block.h"

#include "Chunk.h"
#include "World.h"
#include "Builder.h"

#include <iostream>

//Position Normals TextureCoordinats
const VertexAttributes Block::va(VertexAttribute::Position(), VertexAttribute::Normal(), VertexAttribute::Color(), VertexAttribute::TexCoords());



Block::Block() : material(&Material::AIR), x(0), y(0), z(0), chunk(nullptr), inUpdateList(false) {

}

void Block::tryAddBlockUpdateList() {
    material->tryAddBlockUpdateList(this);
}

void Block::updateTick() {
    material->updateBlock(this);
}

void Block::updateAroundThisBlock() {
    for (int x = 0; x < 3; x++) {
        for (int y = 0; y < 3; y++) {
            for (int z = 0; z < 3; z++) {
                Block* b = getBlock(this->x + x - 1, this->y + y - 1, this->z + z - 1);
                if (b != nullptr) {
                    b->tryAddBlockUpdateList();
                }
            }
        }
    }
}

Block* Block::getBlock(int x, int y, int z) {
    if (x < 0 || y < 0 || z < 0 || x > ChunkSize - 1 || y > ChunkSize - 1 || z > ChunkSize - 1) {
        //Block* b;
        //TestSpeed(b = chunk->chunk->world->getBlock(chunk->chunk->x * ChunkSize + x, chunk->y * ChunkSize + y, chunk->chunk->z * ChunkSize + z, true));
        Block* b = chunk->chunk->world->getBlock(chunk->chunk->x * ChunkSize + x, chunk->y * ChunkSize + y, chunk->chunk->z * ChunkSize + z, false);//TODO true
#ifdef DEBUG
		if(chunk->y > 0 && chunk->y < ChunkHeight) ASSERT(b != nullptr);
#endif
		return b;
    }
    return &chunk->blocks[x][y][z];
}

bool Block::isTransparent(int x, int y, int z) {
    Block* b = getBlock(x, y, z);
    if (b == nullptr) return true;
    return b->material->isTransparent();
}

void Block::updateMesh(bool directUpdate) {
    chunk->updateMeshFromChangedBlock(x, y, z, directUpdate);
}

void Block::addToMesh(Mesh& mesh, std::vector<Collider>& colliders) {
	if (material->getRenderType() == RenderType::None) return;

    Vector3G pos = getPosition();

    bool visible = false;

    if(material->getRenderType() == RenderType::BlockShape) {
        for(BlockFace f : blockFaces) {
            Vector3G normal = getNormal(f);
			Block* nb = getBlock(this->x + (int) normal.x, this->y + (int) normal.y, this->z + (int) normal.z);
			if(nb != nullptr && nb->material->isTransparent()) {
				addFace(f, material->getTextures().getTexture(f), mesh, pos, nb->light);
			}
        }
    }
    else if(material->getRenderType() == RenderType::XShape) {
		addXShape(material->getTextures().front, mesh, pos, light);
    }

    if (visible) {
        //colliders.push_back({ ColliderType::Static, x, y, z, 1, 1, 1 });
    }
}


inline int Block::getAbsoluteX() {
    return x + chunk->chunk->x * ChunkSize;
}

inline int Block::getAbsoluteY() {
    return y + chunk->y * ChunkSize;
}

inline int Block::getAbsoluteZ() {
    return z + chunk->chunk->z * ChunkSize;
}

void Block::addUpdateList() {
    if (!inUpdateList) {
        inUpdateList = true;
        chunk->addUpdateBlocks.push_back(this);
    }
}
void Block::removeUpdateList() {
    if (inUpdateList) {
        inUpdateList = false;
        chunk->removeUpdateBlocks.push_back(this);
    }
}
