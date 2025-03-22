#include "Math.h"

#include <cmath>
#include <stdexcept>

Vector3& operator+=(Vector3& _lhs, const Vector3& _rhs)
{
    _lhs.x += _rhs.x;
    _lhs.y += _rhs.y;
    _lhs.z += _rhs.z;
    return _lhs;
}

Vector3& operator-=(Vector3& _lhs, const Vector3& _rhs)
{
    _lhs.x -= _rhs.x;
    _lhs.y -= _rhs.y;
    _lhs.z -= _rhs.z;
    return _lhs;
}

Vector3& operator*=(Vector3& _vector, const float _scalar)
{
    _vector.x *= _scalar;
    _vector.y *= _scalar;
    _vector.z *= _scalar;
    return _vector;
}

Vector3& operator/=(Vector3& _vector, const float _inverseScalar)
{
    constexpr float epsilon = 1.0e-8f;
    if (std::abs(_inverseScalar) < epsilon)
    {
        return _vector;
    }

    _vector.x /= _inverseScalar;
    _vector.y /= _inverseScalar;
    _vector.z /= _inverseScalar;
    return _vector;
}

Vector3 operator+(const Vector3& _lhs, const Vector3& _rhs)
{
    Vector3 vector(_lhs);
    return vector += _rhs;
}

Vector3 operator-(const Vector3& _lhs, const Vector3& _rhs)
{
    Vector3 vector(_lhs);
    return vector -= _rhs;
}

Vector3 operator*(const Vector3& _vector, const float _scalar)
{
    Vector3 vector(_vector);
    return vector *= _scalar;
}

Vector3 operator/(const Vector3& _vector, const float _inverseScalar)
{
    Vector3 vector(_vector);
    return vector /= _inverseScalar;
}

float math::Dot(const Vector3& _lhs, const Vector3& _rhs)
{
    return _lhs.x * _rhs.x + _lhs.y * _rhs.y + _lhs.z * _rhs.z;
}

Vector3 math::Cross(const Vector3& _lhs, const Vector3& _rhs)
{
    return {
        _lhs.y * _rhs.z - _lhs.z * _rhs.y,
        _lhs.z * _rhs.x - _lhs.x * _rhs.z,
        _lhs.x * _rhs.y - _lhs.y * _rhs.x
    };
}

Vector3 math::Normalize(const Vector3& _vector)
{
    const float length = Length(_vector);
    return _vector / length;
}

float math::Length(const Vector3& _vector)
{
    return std::sqrt(Dot(_vector, _vector));
}