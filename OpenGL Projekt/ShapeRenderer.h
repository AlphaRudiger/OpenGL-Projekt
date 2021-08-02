#pragma once

#include "Mesh.h"
#include "ShaderProgram.h"
#include "Color.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

enum class ShapeType {
	None, Line = GL_LINES, Fill = GL_TRIANGLES
};

class ShapeRenderer {
	Mesh mesh;
	ShaderProgram shader;
	Color color;
	ShapeType type;
public:
	ShapeRenderer();

	void begin(ShapeType type);

	void drawLine(float x1, float y1, float z1, float x2, float y2, float z2);
	void box(float x, float y, float z, float width, float height, float depth);
	void circle(float x, float y, float radius, int parts = 25, int from = 0, int to = 360);

	void setColor(Color color);

	void setTransforamtionMatrix(glm::mat4 matrix);

	void end();
};
