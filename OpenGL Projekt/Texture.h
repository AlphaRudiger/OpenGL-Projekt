#pragma once

#include <string>

class Texture;

class TextureRegion {
protected:
	Texture* texture;

public:
	int x, y, width, height;
	float u, v;
	float u2, v2;

	TextureRegion();
	TextureRegion(Texture* texture);
	TextureRegion(Texture* texture, int x, int y, int width, int height);

	void updateTextureCoords();

	inline Texture* getTexture() {
		return texture;
	}

	inline int getWidth() {
		return width;
	}

	inline int getHeight() {
		return height;
	}
};

class Texture : public TextureRegion {
private:
	unsigned int textureID;
	std::string path;
	unsigned char* buffer;
	int bpp;
	bool init;

public:
	Texture(std::string path, bool flip = true);

	void bind(unsigned int slot = 0);

	inline unsigned char* getDataBuffer() {
		return buffer;
	}

	void initGL();

	~Texture();
};