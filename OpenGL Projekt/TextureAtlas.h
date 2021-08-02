#pragma once

#include <string>
#include <unordered_map>

#include "Texture.h"

class TextureAtlas {
public:
	Texture texture;
	std::unordered_map<std::string, TextureRegion> map;

	TextureAtlas(std::string imagePath, std::string atlasPath);

};