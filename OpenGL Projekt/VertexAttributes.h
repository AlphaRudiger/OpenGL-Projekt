#pragma once

#include "Utils.h"

class VertexAttribute {
public:
private:
	int size;
public:
	//VertexAttribute();
	inline int getSize();
	VertexAttribute(int size);

	inline static VertexAttribute Position() {
		return { 3 };
	}

	inline static VertexAttribute Normal() {
		return { 3 };
	}

	inline static VertexAttribute TexCoords() {
		return { 2 };
	}

	inline static VertexAttribute Color() {
		return { 4 };
	}
};

namespace VAttribute {
	extern VertexAttribute position;
}

class VertexAttributes {
private:
	std::shared_ptr<VertexAttribute> va;
	int size, stride, floatCount;

public:
	template<typename... T>
	VertexAttributes(T... t) : size(sizeof...(t)), floatCount(0) {
		floatCount = (static_cast<VertexAttribute>(t).getSize() + ...);
		va = std::shared_ptr<VertexAttribute>(new VertexAttribute[] { t... });
		stride = floatCount * sizeof(float);
	}
	void layoutCurrentBuffer();
	inline int getFloatCount() {
		return floatCount;
	}
	inline int getAttributeCount() {
		return size;
	}
	inline int getBytesPerVertex() {
		return stride;
	}
	inline VertexAttribute* operator[](int i) {
		return &va.get()[i];
	}
};
