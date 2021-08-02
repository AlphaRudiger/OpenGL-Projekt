#pragma once

#include "Texture.h"
#include "TextureAtlas.h"
#include "General.h"

typedef TextureRegion TR;

extern TR nanTexture;

class TextureContainer {
public:
	TR front;
	TR back;
	TR top;
	TR bottom;
	TR left;
	TR right;

	TextureContainer(TR& texture);
	TextureContainer(TR& front, TR& back, TR& top, TR& bottom, TR& left, TR& right);

	inline TR& getTexture(BlockFace face) {
		switch(face) {
		case BlockFace::Front:
			return front;
		case BlockFace::Back:
			return back;
		case BlockFace::Top:
			return top;
		case BlockFace::Bottom:
			return bottom;
		case BlockFace::Left:
			return left;
		case BlockFace::Right:
			return right;
		}

#ifdef DEBUG
		ASSERT(false);
#endif
		return nanTexture;
	}

};

void loadTextures(TextureAtlas& atlas);