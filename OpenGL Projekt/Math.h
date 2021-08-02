#pragma once

#include "Vector.h"
#include "Matrix.h"
#include "Operations.h"

typedef double NT;

typedef Vec<2, NT> Vector2;
typedef Vec<2, int> Vector2I;
typedef Vec<3, NT> Vector3;
typedef Vec<3, int> Vector3I;
typedef Vec<4, NT> Vector4;
typedef Vec<4, int> Vector4I;

typedef Mat<2, 2, NT> Matrix2x2;
typedef Mat<3, 3, NT> Matrix3x3;
typedef Mat<4, 4, NT> Matrix4x4;

typedef Matrix2x2 Matrix2;
typedef Matrix3x3 Matrix3;
typedef Matrix4x4 Matrix4;