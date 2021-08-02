#pragma once

#include "Utils.h"

enum class RenderType {
	None, BlockShape, XShape
};

enum class BlockFace {
	Front, Back, Left, Right, Top, Bottom
};

const BlockFace blockFaces[] = { BlockFace::Front, BlockFace::Back, BlockFace::Left, BlockFace::Right, BlockFace::Top, BlockFace::Bottom };

inline Vector3G getNormal(BlockFace face) {
	switch(face) {
	case BlockFace::Front:
		return { 0, 0, 1 };
	case BlockFace::Back:
		return { 0, 0, -1 };
	case BlockFace::Top:
		return { 0, 1, 0 };
	case BlockFace::Bottom:
		return { 0, -1, 0 };
	case BlockFace::Left:
		return { -1, 0, 0 };
	case BlockFace::Right:
		return { 1, 0, 0 };
	}
#ifdef DEBUG
	ASSERT(false);
#endif
	return { 0, 0, 0 };
}
