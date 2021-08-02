#pragma once

#include "General.h"
#include "Collision.h"
#include "VertexAttributes.h"
#include "MinecraftTextures.h"
#include "Mesh.h"
#include "Utils.h"
#include "Light.h"

class Block;

typedef void (*BlockUpdateMethod)(Block*);

class Material {
public:
	static Material AIR;
	static Material DIRT;
	static Material GRASS_BLOCK;
	static Material STONE;
	static Material WOOD_LOG;
	static Material GRASS;

private:
	BlockUpdateMethod tryAddUpdateMethod;
	BlockUpdateMethod updateMethod;
	TextureContainer textures;
	RenderType renderType;
	bool transparent;

private:
	inline Material(TextureContainer textures, bool transparent, RenderType renderType = RenderType::BlockShape, BlockUpdateMethod tryAddUpdateMethod = nullptr, BlockUpdateMethod updateMethod = nullptr) : textures(textures), transparent(transparent), tryAddUpdateMethod(tryAddUpdateMethod), updateMethod(updateMethod), renderType(renderType) {

	}

public:
	inline void tryAddBlockUpdateList(Block* block) {
		if (tryAddUpdateMethod != nullptr) {
			tryAddUpdateMethod(block);
		}
	}

	inline void updateBlock(Block* block) {
		if (tryAddUpdateMethod != nullptr) {
			updateMethod(block);
		}
	}

	inline RenderType getRenderType() {
		return renderType;
	}

	inline bool isTransparent() {
		return transparent;
	}

	inline TextureContainer& getTextures() {
		return textures;
	}

	inline void set(TextureContainer textures, bool transparent = false, RenderType renderType = RenderType::BlockShape, BlockUpdateMethod tryAddUpdateMethod = nullptr, BlockUpdateMethod updateMethod = nullptr) {
		this->textures = textures;
		this->transparent = transparent;
		this->updateMethod = updateMethod;
		this->tryAddUpdateMethod = tryAddUpdateMethod;
		this->renderType = renderType;
	}

};

class DataChunk;

class Block {
	
public:
	const static VertexAttributes va;
	Material* material;
	DataChunk* chunk;
	LightLevel light = MAX_LIGHT_LEVEL;
	char x, y, z;
	bool inUpdateList = false;

public:
	Block();
	void addToMesh(Mesh& mesh, std::vector<Collider>& colliders);
	void updateMesh(bool directUpdate = false);
	void tryAddBlockUpdateList();
	void updateTick();
	void updateAroundThisBlock();

	inline void setMaterial(Material& material) {
		this->material = &material;
	}

	inline int getAbsoluteX();

	inline int getAbsoluteY();

	inline int getAbsoluteZ();

	inline Vector3G getPosition() {
		return Vector3G(getAbsoluteX(), getAbsoluteY(), getAbsoluteZ());
	}

	void addUpdateList();
	void removeUpdateList();

	Block* getBlock(int x, int y, int z);
private:
	bool isTransparent(int x, int y, int z);
};