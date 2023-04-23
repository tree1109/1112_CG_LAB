#pragma once
#include "GL\freeglut.h" // freeglut

class vec3
{
public:
    GLfloat x;
    GLfloat y;
    GLfloat z;

    inline vec3& operator+=(const vec3& rhs);
    inline vec3& operator-=(const vec3& rhs);
    inline vec3& operator*=(const GLfloat& rhs);
    GLfloat dot(const vec3& rhs) const;
    vec3 cross(const vec3& rhs) const;
};

inline vec3 operator+(vec3 lhs, const vec3& rhs);
inline vec3 operator-(vec3 lhs, const vec3& rhs);
inline vec3 operator*(vec3 lhs, const GLfloat& rhs);
inline vec3 operator*(const GLfloat& lhs, vec3 rhs);
