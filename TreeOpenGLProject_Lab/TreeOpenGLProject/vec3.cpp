#include "vec3.h"

vec3& vec3::operator+=(const vec3& rhs)
{
    this->x += rhs.x;
    this->y += rhs.y;
    this->z += rhs.z;
    return *this;
}

vec3& vec3::operator-=(const vec3& rhs)
{
    this->x -= rhs.x;
    this->y -= rhs.y;
    this->z -= rhs.z;
    return *this;
}

vec3& vec3::operator*=(const GLfloat& rhs)
{
    this->x *= rhs;
    this->y *= rhs;
    this->z *= rhs;
    return *this;
}

GLfloat vec3::dot(const vec3& rhs) const
{
    vec3 lhs = *this;
    lhs.x *= rhs.x;
    lhs.y *= rhs.y;
    lhs.z *= rhs.z;
    return lhs.x + lhs.y + lhs.z;
}

vec3 vec3::cross(const vec3& rhs) const
{
    const vec3& lhs = *this;
    return {
        lhs.y * rhs.z - lhs.z * rhs.y,
        lhs.z * rhs.x - lhs.x * rhs.z,
        lhs.x * rhs.y - lhs.y * rhs.x
    };
}

inline vec3 operator+(vec3 lhs, const vec3& rhs) { return lhs += rhs; }
inline vec3 operator-(vec3 lhs, const vec3& rhs) { return lhs -= rhs; }
inline vec3 operator*(vec3 lhs, const GLfloat& rhs) { return lhs *= rhs; }
inline vec3 operator*(const GLfloat& lhs, vec3 rhs) { return rhs *= lhs; }
