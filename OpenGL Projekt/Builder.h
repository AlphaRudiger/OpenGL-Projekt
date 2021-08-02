#pragma once

#include "Mesh.h"
#include "Texture.h"
#include "Block.h"
#include "Light.h"

void addFace(BlockFace face, TextureRegion& tr, Mesh& mesh, Vector3G& pos, LightLevel light = MAX_LIGHT_LEVEL);
void addXShape(TextureRegion& tr, Mesh& mesh, Vector3G& pos, LightLevel light = MAX_LIGHT_LEVEL);
