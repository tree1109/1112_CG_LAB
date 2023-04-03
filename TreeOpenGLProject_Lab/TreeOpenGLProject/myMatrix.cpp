#include <iostream>
#include "GL\freeglut.h" // freeglut
#include "myMatrix.h"
#define M_PI 3.1415926535897932384626433832795f

myMatrix::myMatrix(void):
    deg2rad(M_PI / 180.0f)
{
    ResetMatrix();
}

myMatrix::~myMatrix(void)
{
}

void myMatrix::ResetMatrix(void)
{
    for (int i = 0; i < 4; i++) {
        matrix[4 * i] = i == 0 ? 1.0f : 0.0f;
        matrix[4 * i + 1] = i == 1 ? 1.0f : 0.0f;
        matrix[4 * i + 2] = i == 2 ? 1.0f : 0.0f;
        matrix[4 * i + 3] = i == 3 ? 1.0f : 0.0f;
    }
}

void myMatrix::TranslateMatrix(GLfloat x, GLfloat y, GLfloat z)
{
    ResetMatrix();

    matrix[12] = x;
    matrix[13] = y;
    matrix[14] = z;

    glMultMatrixf(matrix);
}

void myMatrix::RotateMatrix(GLfloat angle, GLfloat ux, GLfloat uy, GLfloat uz)
{
    GLfloat RaCos = cos(angle * deg2rad);
    GLfloat RaSin = sin(angle * deg2rad);

    ResetMatrix();

    // Rodrigues' rotation formula
    matrix[0] = RaCos + (1 - RaCos) * ux * ux;
    matrix[1] = (1 - RaCos) * ux * uy + RaSin * uz;
    matrix[2] = (1 - RaCos) * ux * uz - RaSin * uy;
    matrix[4] = (1 - RaCos) * ux * uy - RaSin * uz;
    matrix[5] = RaCos + (1 - RaCos) * uy * uy;
    matrix[6] = (1 - RaCos) * uy * uz + RaSin * ux;
    matrix[8] = (1 - RaCos) * ux * uz + RaSin * uy;
    matrix[9] = (1 - RaCos) * uy * uz - RaSin * ux;
    matrix[10] = RaCos + (1 - RaCos) * uz * uz;

    glMultMatrixf(matrix);
}

void myMatrix::ArbitraryRotate(GLfloat angle, GLfloat p1[], GLfloat p2[])
{
    // get unit vector
    GLfloat length = sqrt((p2[0] - p1[0]) * (p2[0] - p1[0]) + (p2[1] - p1[1]) * (p2[1] - p1[1]) + (p2[2] - p1[2]) * (p2[2] - p1[2]));
    GLfloat x = (p2[0] - p1[0]) / length;
    GLfloat y = (p2[1] - p1[1]) / length;
    GLfloat z = (p2[2] - p1[2]) / length;

    TranslateMatrix(p1[0], p1[1], p1[2]);    // move origin of model space to origin of unit vector
    RotateMatrix(angle, x, y, z);            // rotate at origin of unit vector
    TranslateMatrix(-p1[0], -p1[1], -p1[2]); // reverse move
}
