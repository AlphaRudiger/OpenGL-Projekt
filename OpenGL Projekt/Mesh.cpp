#include "Mesh.h"

#include <cstring>
#include <utility>
#include <iostream>

#include "Utils.h"

using namespace std;

VertexData::VertexData(int size, VertexAttributes va) : va(va), size(0), maxSize(size), update(true), vertices(nullptr), init(false), buffer(0), vao(0) {
	if (maxSize > 0) {
		vertices = new float[size * va.getFloatCount()];
	}
}

void VertexData::setVertices(float* vertices, int size) {
	clear();
	addVertices(vertices, size);
}

void VertexData::addVertices(float* vertices, int size) {
	ensureCapacity(size);
	memcpy(this->vertices + this->size * va.getFloatCount(), vertices, size * va.getBytesPerVertex());
	this->size += size;
	update = true;
}

void VertexData::ensureCapacity(int size) {
	if (this->size + size > this->maxSize) {
		if (this->vertices == nullptr) {
			changeSize(max(DEFAULT_VERTEX_BUFFER_SIZE, size));
			return;
		} else {
			changeSize(maxSize * 2 + size);
			cout << "Resize buffer" << endl;
		}

	}
}

void VertexData::bind() {
	if (!init) {
		init = true;
		GLCall(glGenVertexArrays(1, &vao));
		GLCall(glBindVertexArray(vao));
		GLCall(glGenBuffers(1, &buffer));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
		va.layoutCurrentBuffer();
	}

	GLCall(glBindVertexArray(vao));
	if (update) {
		update = false;
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
		GLCall(glBufferData(GL_ARRAY_BUFFER, this->size * va.getBytesPerVertex(), this->vertices, GL_DYNAMIC_DRAW));
	}
}

void VertexData::changeSize(int newSize) {
	if (newSize != maxSize) {
		int allo = newSize * va.getFloatCount();
		float* newArray = new float[allo];
		if (this->vertices != nullptr) {
			memcpy(newArray, this->vertices, this->size * va.getBytesPerVertex());
			delete[] this->vertices;
		}
		this->vertices = newArray;
		this->maxSize = newSize;
	}
}

void VertexData::draw(GLenum mode) {
	bind();
	glDrawArrays(mode, 0, size);
}

VertexData::~VertexData() {
	if(vertices != nullptr) delete[] vertices;
	if(init) glDeleteBuffers(1, &buffer);
}



IndexData::IndexData(int size) : maxSize(size), size(0), update(true), indices(nullptr), init(false), buffer(0) {
	if (size > 0) {
		indices = new IndiceType[size];
	}
}

void IndexData::setIndices(IndiceType* indices, int size) {
	clear();
	addIndices(indices, size);
}

void IndexData::addIndices(IndiceType* indices, int size) {
	ensureCapacity(size);
	memcpy(this->indices + this->size, indices, size * sizeof(IndiceType));
	this->size += size;
	update = true;
}

void IndexData::bind() {
	if (!init) {
		init = true;
		GLCall(glGenBuffers(1, &buffer));
	}
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer));
	if (update) {
		update = false;
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->size * sizeof(IndiceType), this->indices, GL_DYNAMIC_DRAW));
	}
}

void IndexData::ensureCapacity(int size) {
	if (this->size + size > this->maxSize) {
		if (this->indices == nullptr) {
			changeSize(max(DEFAULT_Indice_BUFFER_SIZE, size));
		} else {
			changeSize(maxSize * 2 + size);
		}

	}
}

void IndexData::changeSize(int newSize) {
	if(newSize != maxSize) {

		IndiceType* newArray = new IndiceType[newSize];
		if (this->indices != nullptr) {
			memcpy(newArray, this->indices, this->size * sizeof(IndiceType));
			delete[] this->indices;
		}
		this->indices = newArray;
		this->maxSize = newSize;
	}
}

IndexData::~IndexData() {
	if(indices != nullptr) delete[] indices;
	if(init) GLCall(glDeleteBuffers(1, &buffer));
}

Mesh::Mesh(VertexAttributes va) : Mesh(0, 0, va) {

}

Mesh::Mesh(int maxVertices, int maxIndices, VertexAttributes va) : vertices(maxVertices, va), indices(maxIndices) {
	
}

void Mesh::setVertices(float* vertices, int size) {
	this->vertices.setVertices(vertices, size);
}

void Mesh::setIndices(IndiceType* indices, int size) {
	this->indices.setIndices(indices, size);
}

void Mesh::draw(GLenum mode) {
	if (vertices.getSize() > 0 && indices.getSize() > 0) {
		vertices.bind();
		indices.bind();
		GLCall(glDrawElements(mode, indices.getIndiceCount(), GL_UNSIGNED_SHORT, nullptr));
	}
}

void Mesh::addMesh(float* vertices, int verticesSize, IndiceType* indices, int indicesSize) {
	int vs = this->vertices.getSize();
	this->vertices.addVertices(vertices, verticesSize);
	int s = this->indices.getSize();
	this->indices.addIndices(indices, indicesSize);
	for (int i = 0; i < indicesSize; i++) {
		this->indices.indices[i + s] += vs;
	}
}

void Mesh::trim() {
	vertices.changeSize(vertices.getSize());
	indices.changeSize(indices.getSize());
}

void Mesh::print() {
	std::cout << "Mesh:\n";
	std::cout << "Vertices:\n";
	VertexAttributes va = vertices.getVertexAttributes();
	int att = va.getAttributeCount();
	int fc = va.getFloatCount();
	int f = 0;
	for (int i = 0; i < vertices.getSize(); i++) {
		cout << "Vertice " << i << ": ";
		for (int a = 0; a < att; a++) {
			std::cout << "{";
			for (int j = 0; j < va[a]->getSize(); j++) {
				std::cout << vertices.vertices[f++];
				if (j != va[a]->getSize() - 1) {
					std::cout << " ";
				}
			}
			std::cout << "} ";
		}
		std::cout << std::endl;
	}

	std::cout << "Indices:\n";

	for (int i = 1; i < indices.getSize() + 1; i++) {
		cout << indices.indices[i - 1] << " ";
		if (i % va[0]->getSize() == 0) cout << endl;
	}
	
}

Mesh::~Mesh() {

}