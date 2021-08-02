#pragma once

#include "Vector.h"
template<size_t Rows, size_t Cols, typename T>
struct Mat {
	typedef Mat<Rows, Cols, T> Type;
	typedef Vec<Rows, T> ColType;
	typedef Vec<Cols, T> RowType;

	ColType columns[Cols];

	inline ColType operator*(RowType& v) {
		ColType r = {};

		for(size_t i = 0; i < Cols; i++) {
			r += columns[i] * v[i];
		}
		//for(size_t i = 0; i < Rows; i++) {
		//	for(size_t j = 0; j < Cols; j++) {
		//		r[i] += columns[j][i] * v[j];
		//	}
		//}
		return r;
	}

	inline Type operator*(Type& m) {
		Type r = { };
		for(size_t i = 0; i < Cols; i++) {
			r[i] = *this * m[i];
		}
		return r;
	}

	Type& idk() {
		Type copy = *this;
		T* t = &copy[0][0];
		int c = 0;
		for(size_t i = 0; i < Rows; i++) {
			for(size_t j = 0; j < Cols; j++) {
				columns[j][i] = t[c++];
			}
		}
		for(size_t i = 0; i < Rows * Cols; i++) {
		}
		return *this;
	}

	inline ColType& operator[](size_t i) {
		return columns[i];
	}

	inline T det() noexcept requires (Rows == 2 && Cols == 2) {
		return columns[0][0] * columns[1][1] - columns[0][1] * columns[1][0];
	}

};

template<size_t Rows, size_t Cols, typename T>
std::ostream& operator<<(std::ostream& o, Mat<Rows, Cols, T> mat) {
	o << "Matrix" << Rows << "x" << Cols << ':' << endl;
	for(int i = 0; i < Rows; i++) {
		cout << '{';
		for(int j = 0; j < Cols; j++) {
			o << mat.columns[j][i];
			if(j != Cols - 1) {
				cout << ", ";
			}
		}
		cout << '}' << endl;
	}
	return o;
}
