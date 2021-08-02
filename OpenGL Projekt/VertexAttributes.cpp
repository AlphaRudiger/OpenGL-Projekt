#include "VertexAttributes.h"

#include <cstring>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

//VertexAttribute::VertexAttribute() : size(0) {
//
//}

inline int VertexAttribute::getSize() {
	return size;
}

VertexAttribute::VertexAttribute(int size) : size(size) {

}


void VertexAttributes::layoutCurrentBuffer() {
	int p = 0;
	for (int i = 0; i < size; i++) {
		const int s = va.get()[i].getSize();
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, s, GL_FLOAT, GL_FALSE, stride, (void*) (long long) p);
		p += s * sizeof(float);
	}
}