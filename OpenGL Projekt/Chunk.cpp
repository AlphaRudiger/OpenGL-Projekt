#include "Chunk.h"

#include "World.h"

DataChunk::DataChunk() : DataChunk(0, -1) {

}

DataChunk::DataChunk(Chunk* chunk, int y) : y(y), chunk(chunk), mesh(0, 0, Block::va), tmesh(0, 0, Block::va) {
	for (int x = 0; x < ChunkSize; x++) {
		for (int y = 0; y < ChunkSize; y++) {
			for (int z = 0; z < ChunkSize; z++) {
				Block& b = blocks[x][y][z];
				b.x = x;
				b.y = y;
				b.z = z;
				b.chunk = this;
			}
		}
	}
}

void DataChunk::addLight(LightNode light) {
	light.pos.x = light.pos.x - getChunkNumber(light.pos.x) * ChunkSize;
	light.pos.y = light.pos.y - getChunkNumber(light.pos.y) * ChunkSize;
	light.pos.z = light.pos.z - getChunkNumber(light.pos.z) * ChunkSize;
	
	lights.push_back(light);
}

void DataChunk::updateMesh() {
	if (!updatedMesh) {
		updatedMesh = true;
		colliders.clear();
		mesh.clear();
		tmesh.clear();
		for (int x = 0; x < ChunkSize; x++) {
			for (int y = 0; y < ChunkSize; y++) {
				for (int z = 0; z < ChunkSize; z++) {
					blocks[x][y][z].addToMesh(blocks[x][y][z].material->isTransparent() ? tmesh : mesh, colliders);
				}
			}
		}
	}
}

void DataChunk::updateMeshFromChangedBlock(int x, int y, int z, bool directUpdate) {
	Block* b = &blocks[x][y][z];
	b->updateAroundThisBlock();
	this->meshNeedsUpdate();
	if (directUpdate) this->updateMesh();
	Block* gb;
	DataChunk* c;
#define Check(ax, ay, az)	gb = b->getBlock(x + ax, y + ay, z + az);\
							if(gb != nullptr) {\
								c = gb->chunk;\
								if(c != this) {\
									c->meshNeedsUpdate();\
									if(directUpdate) TestSpeed(c->updateMesh());\
								}\
							}

	Check(1, 0, 0);
	Check(-1, 0, 0);
	Check(0, 1, 0);
	Check(0, -1, 0);
	Check(0, 0, 1);
	Check(0, 0, -1);
}

void DataChunk::draw() {
	if(mesh.vertices.getSize()) {
		GLCall(glDisable(GL_BLEND));
		mesh.draw(GL_TRIANGLES);
	}
	if(tmesh.vertices.getSize()) {
		//GLCall(glEnable(GL_BLEND));
		tmesh.draw(GL_TRIANGLES);
	}
}

Chunk::Chunk() : Chunk(0, 0) {

}

Chunk::Chunk(int x, int z) : x(x), z(z), world(nullptr), boundingBox({ x * ChunkSize, 0, z * ChunkSize }, { x * ChunkSize + ChunkSize, ChunkSize * ChunkHeight, z * ChunkSize + ChunkSize }) {
	//std::cout << "Create Chunk: " << x << " " << z << std::endl;
	for (int i = 0; i < ChunkHeight; i++) {
		chunks[i].chunk = this;
		chunks[i].y = i;
	}
}

Block* Chunk::getBlock(int x, int y, int z) {
	int cc = y / ChunkSize;
	int ry = y - cc * ChunkSize;
	if (cc < 0 || cc > ChunkHeight - 1 || x < 0 || ry < 0 || z < 0 || x > ChunkSize - 1 || ry > ChunkSize - 1 || z > ChunkSize - 1) {
#ifdef DEBUG
		std::cout << "get Block out of bounds: " << std::endl;
		std::cout << "Chunk: " << this->x << " " << this->z << std::endl;
		std::cout << "Data Chunk: " << cc << " Max " << ChunkHeight - 1 << std::endl;
		std::cout << x << " " << ry << " " << z << std::endl;
#endif
		return nullptr;
	}
	DataChunk& c = chunks[cc];
	return &c.blocks[x][ry][z];
}

void Chunk::updateMeshFromChangedBlock(int x, int y, int z) {
	DataChunk* c = getDataChunk(y);
	if (c != nullptr) c->updateMeshFromChangedBlock(x, y - c->y * ChunkSize, z);
}

void Chunk::firstUpdateTick() {
	for (int x = 0; x < ChunkSize; x++) {
		for (int y = 0; y < ChunkSize * ChunkHeight; y++) {
			for (int z = 0; z < ChunkSize; z++) {
				getBlock(x, y, z)->tryAddBlockUpdateList();
			}
		}
	}
}

//void Chunk::updateTick() {
//	for (int x = 0; x < ChunkSize; x++) {
//		for (int y = 0; y < ChunkSize * ChunkHeight; y++) {
//			for (int z = 0; z < ChunkSize; z++) {
//				getBlock(x, y, z)->updateTick();
//			}
//		}
//	}
//}

void Chunk::updateMesh() {
	//std::cout << "UpdateMesh: " << x << " " << z << std::endl;
	for (int i = 0; i < ChunkHeight; i++) {
		chunks[i].updateMesh();
	}
}

void Chunk::draw() {
	for (int i = 0; i < ChunkHeight; i++) {
		chunks[i].draw();
	}
}

Chunk::~Chunk() {
	for (Entity* e : entitys) {
		delete e;
	}
	//std::cout << "Delete Chunk: " << this->x << " " << this->z << std::endl;
}