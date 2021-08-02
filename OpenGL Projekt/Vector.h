#pragma once

#include <cstddef>

#include <iostream>

using std::cout;
using std::endl;

template<size_t N, typename T>
struct Vec {
	T values[N];

	typedef Vec<N, T> Type;

	inline T& x() noexcept requires (N >= 1)  {
		return values[0];
	}
	inline T& y() noexcept requires (N >= 2) {
		return values[1]; 
	}
	inline T& z() noexcept requires (N >= 3) {
		return values[2];
	}
	inline T& w() noexcept requires (N >= 4) {
		return values[3];
	}

	inline T& operator[](size_t i) {
		return values[i];
	}

	inline Type operator+(Type v) {
		Type r;
		for(int i = 0; i < N; i++) {
			r[i] = values[i] + v[i];
		}
		return r;
	}
	inline void operator+=(Type v) {
		for(int i = 0; i < N; i++) {
			values[i] += v[i];
		}
	}
	inline Type operator-(Type v) {
		Type r;
		for(int i = 0; i < N; i++) {
			r[i] = values[i] - v[i];
		}
		return r;
	}
	inline Type operator*(T scl) {
		Type r;
		for(size_t i = 0; i < N; i++) {
			r[i] = values[i] * scl;
		}
		return r;
	}
	inline Type operator*(Type t) {
		Type r;
		for(size_t i = 0; i < N; i++) {
			r[i] = values[i] * t[i];
		}
		return r;
	}
};

template<size_t N, typename T>
std::ostream& operator<<(std::ostream& o, Vec<N, T> v) {
	o << '{';
	for(int i = 0; i < N; i++) {
		cout << v[i];
		if(i != N - 1) {
			cout << ", ";
		}
	}
	o << '}';
	return o;
}




