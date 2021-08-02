#include "Texture.h"

#include <stb_image.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

TextureRegion::TextureRegion() : TextureRegion(nullptr, 0, 0, 0, 0) {

}
TextureRegion::TextureRegion(Texture* texture, int x, int y, int width, int height) : texture(texture), x(x), y(y), width(width), height(height) {
	if (texture) {
		updateTextureCoords();
	}
}

void TextureRegion::updateTextureCoords() {
	float invTexWidth = 1.0f / texture->getWidth();
	float invTexHeight = 1.0f / texture->getHeight();
	u = x * invTexWidth;
	v = y * invTexHeight;
	u2 = (x + width) * invTexWidth;
	v2 = (y + height) * invTexHeight;
	//u = x / texture->getWidth();
	//u2 = (x + width) / texture->getWidth();
	//v = y / texture->getHeight();
	//v2 = (y + height) / texture->getHeight();
}

Texture::Texture(std::string path, bool flip) : path(path), textureID(0) {
	stbi_set_flip_vertically_on_load(flip);
	buffer = stbi_load(path.c_str(), &width, &height, &bpp, 4);
	
	if (buffer == nullptr) {
		std::cout << "Faild loading img" << std::endl;
	}

	this->texture = this;
	updateTextureCoords();
	

}

void Texture::initGL() {
	init = true;
	glGenTextures(1, &textureID);
	bind();
	//for (int i = 0; i < width * height * 4; i++) {
	//	buffer[i] = 255;
	//}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
}

void Texture::bind(unsigned int slot) {
	if(!init) {
		initGL();
	}
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, textureID);
}

Texture::~Texture() {
	if(init) glDeleteTextures(1, &textureID);
	stbi_image_free(buffer);
}