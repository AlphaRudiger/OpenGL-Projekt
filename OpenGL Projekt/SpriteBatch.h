#pragma once

#include "Mesh.h"
#include "Texture.h"
#include "ShaderProgram.h"
#include "Utils.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

class SpriteBatch {
	Mesh mesh;
	float r, g, b, a;
	ShaderProgram shader;
	Texture* currentTexture;
	
public:
	SpriteBatch();

	void setColor(float r, float g, float b, float a);
	void begin();
	void setTransformationMatrix(Matrix4G matrix);
	void drawTexture(TextureRegion& texture, float x, float y, float width, float height);
	void drawTexture(TextureRegion& texture, float x1, float y1, float z1, float x2, float y2, float z2);
	void end();
};