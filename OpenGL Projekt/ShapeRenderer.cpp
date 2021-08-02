#include "ShapeRenderer.h"

#include "VertexAttributes.h"

#define _USE_MATH_DEFINES
#include <math.h>

ShapeRenderer::ShapeRenderer() : mesh({ {3, 4} }), shader("assets\\el\\shader\\ShapeRenderer\\VertexShader.glsl", "assets\\el\\shader\\ShapeRenderer\\FragmentShader.glsl", true), color(Color::WHITE), type(ShapeType::None) {
	
}

#define VAC color.r, color.g, color.b, color.a

void ShapeRenderer::begin(ShapeType type) {
	shader.bind();
	mesh.clear();
	this->type = type;
}

void ShapeRenderer::setColor(Color color) {
	this->color = color;
}

void ShapeRenderer::circle(float x, float y, float radius, int parts, int from, int to) {
	float angle = M_PI * 2 / parts;
	float lx = 0;
	float ly = 0;
	for(int i = from / 360.f * parts; i <= to / 360.f * parts; i++) {
		float nx = radius * sin(angle * i);
		float ny = radius * cos(angle * i);
		if(lx != 0 || ly != 0) {
			if(type == ShapeType::Fill) {
				float v[] = { x, y, 0, VAC, nx + x, ny + y, 0, VAC, lx + x, ly + y, 0, VAC };
				IndiceType in[] = { 0, 1, 2 };
				mesh.addMesh(v, 3, in, 3);
			}
			else if(type == ShapeType::Line) {
				float v[] = { lx + x, ly + y, 0, VAC, nx + x, ny + y, 0, VAC };
				IndiceType in[] = { 0, 1 };
				mesh.addMesh(v, 2, in, 2);
			}
		}
		lx = nx;
		ly = ny;
	}
}

void ShapeRenderer::drawLine(float x1, float y1, float z1, float x2, float y2, float z2) {
	float v[]{
		x1, y1, z1, VAC,
		x2, y2, z2, VAC
	};
	IndiceType i[2]{0, 1};
	mesh.addMesh(v, 2, i, 2);
}

void ShapeRenderer::box(float x, float y, float z, float width, float height, float depth) {
	float v[]{
		x, y, z, VAC,
		x + width, y, z, VAC,
		x + width, y, z + depth, VAC,
		x, y, z + depth, VAC,

		x, y + height, z, VAC,
		x + width, y + height, z, VAC,
		x + width, y + height, z + depth, VAC,
		x, y + height, z + depth, VAC,
	};
	if (type == ShapeType::Line) {
		IndiceType indices[]{
			0, 1,
			1, 2,
			2, 3,
			3, 0,

			4, 5,
			5, 6,
			6, 7,
			7, 4,
			
			0, 4,
			1, 5,
			2, 6,
			3, 7
		};

		mesh.addMesh(v, 8, indices, 24);
	} else {
		IndiceType indices[]{
			5, 1, 0,
			0, 4, 5,
			6, 2, 1,
			1, 5, 6,
			7, 3, 2,
			2, 6, 7,
			0, 3, 7,
			7, 4, 0,
			6, 5, 4,
			4, 7, 6,
			0, 1, 2,
			2, 3, 0

		};
		mesh.addMesh(v, 8, indices, 36);
	}
}

void ShapeRenderer::setTransforamtionMatrix(glm::mat4 matrix) {
	shader.bind();
	shader.setUniformMat4f("matrix", matrix);
}

void ShapeRenderer::end() {
	mesh.draw((unsigned int) type);
}