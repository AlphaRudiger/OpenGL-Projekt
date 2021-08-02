#include "SpriteBatch.h"

#define color r, g, b, a

SpriteBatch::SpriteBatch() : mesh({ {2, 4, 2} }), shader("assets\\el\\shader\\SpriteBatch\\VertexShader.glsl", "assets\\el\\shader\\SpriteBatch\\FragmentShader.glsl", true), r(1), g(1), b(1), a(1), currentTexture(nullptr) {
	shader.bind();
	setTransformationMatrix(glm::ortho(0.f, 1920.f, 0.f, 1080.f));
}

void SpriteBatch::setColor(float r, float g, float b, float a) {
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}

void SpriteBatch::begin() {
	mesh.clear();
	shader.bind();
}

void SpriteBatch::drawTexture(TextureRegion& texture, float x, float y, float width, float height) {
	if (currentTexture != texture.getTexture()) {
		if (mesh.vertices.getSize()) {
			end();
			begin();
		}
		currentTexture = texture.getTexture();
		texture.getTexture()->bind();	
	}
	float v[]{
		x, y, color, texture.u, texture.v,
		x + width, y, color, texture.u2, texture.v,
		x + width, y + height, color, texture.u2, texture.v2,
		x, y + height, color, texture.u, texture.v2,
	};
	IndiceType in[]{
		0, 1, 2,
		2, 3, 0
	};
	mesh.addMesh(v, 4, in, 6);
}

void SpriteBatch::drawTexture(TextureRegion& texture, float x1, float y1, float z1, float x2, float y2, float z2) {
	if (currentTexture != texture.getTexture()) {
		if (mesh.vertices.getSize()) {
			end();
			begin();
		}
	}
	float v[]{
		x1, y1, z1, color, texture.u, texture.v,
		x2 - x1, y1, z1, color, texture.u, texture.v,
		x2 - x1, y1, z1, color, texture.u, texture.v,
		x1, y1, z1, color, texture.u, texture.v,
	};
}

void SpriteBatch::setTransformationMatrix(Matrix4G matrix) {
	shader.setUniformMat4f("matrix", matrix);
}

void SpriteBatch::end() {
	mesh.draw(GL_TRIANGLES);
}