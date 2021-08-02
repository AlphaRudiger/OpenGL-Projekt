#pragma once

#include "Vector.h"
#include "Matrix.h"

#include <numeric>

template<size_t N, typename T>
inline T sum(Vec<N, T> v) {
	return reduce(v.values, v.values + N);
}

template<size_t N, typename T>
inline T len(Vec<N, T> v) {
	T idk = 0;
	for(size_t i = 0; i < N; i++) {
		idk += v[i] * v[i];
	}
	return sqrt(idk);
}

template<size_t N, typename T>
inline Vec<N, T> normalize(Vec<N, T> v) {
	T len = sum(v);
	Vec<N, T> r;
	for(size_t i = 0; i < N; i++) {
		r[i] = v[i] / len;
	}
	return r;
}

template<typename T>
Vec<3, T> cross(Vec<3, T> v1, Vec<3, T> v2) {
	//Mat<3, 3, T> mat = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };
	//Mat<2, 2, T> mi = { v1.y(), v1.z(), v2.y(), v2.z() };
	//Mat<2, 2, T> mj = { v1.x(), v1.z(), v2.x(), v2.z() };
	//Mat<2, 2, T> mk = { v1.x(), v1.y(), v2.x(), v2.y() };
	//return (mat[0] * mi.det()) - (mat[1] * mj.det()) + (mat[2] * mk.det());
	return { 
		v1.y() * v2.z() - v1.z() * v2.y(),
		v1.z() * v2.x() - v1.x() * v2.z(),
		v1.x() * v2.y() - v1.y() * v2.x()
	};
}

template<size_t N, typename T>
T dot(Vec<N, T> v1, Vec<N, T> v2) {
	return sum(v1 * v2);
}

