#pragma once

#ifndef INCLUDE_MATH_H
#define INCLUDE_MATH_H

#include "Vector3.h"

extern Vector3& operator+=(Vector3& _lhs, const Vector3& _rhs);
extern Vector3& operator-=(Vector3& _lhs, const Vector3& _rhs);
extern Vector3& operator*=(Vector3& _vector, const float _scalar);
extern Vector3& operator/=(Vector3& _vector, const float _inverseScalar);
extern Vector3 operator+(const Vector3& _lhs, const Vector3& _rhs);
extern Vector3 operator-(const Vector3& _lhs, const Vector3& _rhs);
extern Vector3 operator*(const Vector3& _vector, const float _scalar);
extern Vector3 operator/(const Vector3& _vector, const float _inverseScalar);

namespace math
{
    // Vector3.
    extern float Dot(const Vector3& _lhs, const Vector3& _rhs);
    extern Vector3 Cross(const Vector3& _lhs, const Vector3& _rhs);
    extern Vector3 Normalize(const Vector3& _vector);
    extern float Length(const Vector3& _vector);
}

#endif // !INCLUDE_MATH_H