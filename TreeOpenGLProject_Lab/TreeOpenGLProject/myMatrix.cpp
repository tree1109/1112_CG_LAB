#include <iostream>
#include "GL\freeglut.h" // freeglut
#include "myMatrix.h"
#define M_PI 3.1415926535897932384626433832795f

myMatrix::myMatrix(void)
{
    ResetMatrix();
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

myMatrix myMatrix::mult(const myMatrix& rightM) const
{
    myMatrix resultM;

    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            GLfloat sum = 0.0f;
            sum += matrix[4 * row] * rightM.matrix[col];
            sum += matrix[4 * row + 1] * rightM.matrix[4 + col];
            sum += matrix[4 * row + 2] * rightM.matrix[8 + col];
            sum += matrix[4 * row + 3] * rightM.matrix[12 + col];
            resultM.matrix[4 * row + col] = sum;
        }
    }

    return resultM;
}

vec3 myMatrix::mult(const vec3& rightV) const
{
    vec3 resultV;

    resultV.x = matrix[0] * rightV.x + matrix[4] * rightV.y + matrix[8] * rightV.z;
    resultV.y = matrix[1] * rightV.x + matrix[5] * rightV.y + matrix[9] * rightV.z;
    resultV.z = matrix[2] * rightV.x + matrix[6] * rightV.y + matrix[10] * rightV.z;

    return resultV;
}

void myMatrix::setTranslateMatrix(GLfloat x, GLfloat y, GLfloat z)
{
    ResetMatrix();

    matrix[12] = x;
    matrix[13] = y;
    matrix[14] = z;
}

void myMatrix::setRotateMatrix(GLfloat angle, GLfloat ux, GLfloat uy, GLfloat uz)
{
    constexpr GLfloat deg2rad(M_PI / 180.0f);
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
}

void myMatrix::setScaleMatrix(GLfloat sx, GLfloat sy, GLfloat sz)
{
    ResetMatrix();
    matrix[0] = sx;
    matrix[5] = sy;
    matrix[10] = sz;
}

void myMatrix::setFrustumMatrix(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar)
{
    ResetMatrix();
    matrix[0] = 2 * zNear / (right - left);
    matrix[5] = 2 * zNear / (top - bottom);
    matrix[8] = (right + left) / (right - left);
    matrix[9] = (top + bottom) / (top - bottom);
    matrix[10] = 0 - (zFar + zNear) / (zFar - zNear);
    matrix[11] = 0 - 1;
    matrix[14] = 0 - 2 * zFar * zNear / (zFar - zNear);
    matrix[15] = 0;
}

void myMatrix::doTranslate(GLfloat x, GLfloat y, GLfloat z)
{
    setTranslateMatrix(x, y, z);
    glMultMatrixf(matrix);
}

void myMatrix::doRotate(GLfloat angle, GLfloat ux, GLfloat uy, GLfloat uz)
{
    setRotateMatrix(angle, ux, uy, uz);
    glMultMatrixf(matrix);
}

void myMatrix::doArbitraryRotate(GLfloat angle, vec3 v1, vec3 v2)
{
    // get unit vector
    vec3 unitV = (v2 - v1).normalize();

    doTranslate(v1.x, v1.y, v1.z);    // move origin of model space to origin of unit vector
    doRotate(angle, unitV.x, unitV.y, unitV.z);      // rotate at origin of unit vector
    doTranslate(-v1.x, -v1.y, -v1.z); // reverse move
}

void myMatrix::doScale(GLfloat scale)
{
    setScaleMatrix(scale, scale, scale);
    glMultMatrixf(matrix);
}

void myMatrix::doScale(GLfloat sx, GLfloat sy, GLfloat sz)
{
    setScaleMatrix(sx, sy, sz);
    glMultMatrixf(matrix);
}

void myMatrix::doFrustum(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar)
{
    setFrustumMatrix(left, right, bottom, top, zNear, zFar);
    glMultMatrixf(matrix);
}
