#pragma once

#include "VertexAttributes.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define DEFAULT_VERTEX_BUFFER_SIZE 10000
#define DEFAULT_Indice_BUFFER_SIZE 10000

typedef unsigned short IndiceType;

class VertexData {
private:
	VertexAttributes va;
	unsigned int buffer, vao;
	int maxSize, size, drawSize;
	bool update, init;

public:
	float* vertices;
	VertexData(int size, VertexAttributes va);
	
	void setVertices(float* vertices, int size);
	void addVertices(float* vertices, int size);
	inline void clear(bool idk = false) {
		if(idk) drawSize = size;
		size = 0;
		update = true;
	}
	inline void idk() {
		drawSize = size;
	}
	void bind();
	void draw(GLenum mode);
	void ensureCapacity(int size);
	void changeSize(int size);

	inline int getSize() {
		return size;
	}

	inline VertexAttributes& getVertexAttributes() {
		return va;
	}
	~VertexData();

};

class IndexData {
private:
	unsigned int buffer;
	int maxSize, size, drawSize;
	bool update, init;

public:
	IndiceType* indices;
	IndexData(int size);

	void setIndices(IndiceType* indices, int size);
	void addIndices(IndiceType* indices, int size);
	inline void clear(bool idk = false) {
		if(idk) drawSize = size;
		size = 0;
		update = true;
	}
	inline void idk() {
		drawSize = size;
	}
	inline int getSize() {
		return size;
	}
	void ensureCapacity(int size);

	void changeSize(int size);

	void bind();
	inline int getIndiceCount() {
		return size;
	}

	~IndexData();

};

class Mesh {
public:
	VertexData vertices;
	IndexData indices;

public:
	Mesh(VertexAttributes va);
	Mesh(int maxVertices, int maxIndices, VertexAttributes va);

	void setVertices(float* vertices, int size);
	void setIndices(IndiceType* indices, int size);
	void draw(GLenum mode);
	void addMesh(float* vertices, int verticesSize, IndiceType* indices, int indicesSize);
	inline void clear(bool idk = false) {
		indices.clear(idk);
		vertices.clear(idk);
	}
	inline void idk() {
		indices.idk();
		vertices.idk();
	}
	void print();
	void trim();

	inline VertexAttributes& getVertexAttributes() {
		return vertices.getVertexAttributes();
	}

	~Mesh();
};