#pragma once

#include "Math.h"

class myMatrix
{
public:
    myMatrix(void);
    void ResetMatrix(void);
    myMatrix mult(const myMatrix& rightM) const;
    Vector3 mult(const Vector3 &rightV) const;
    void setTranslateMatrix(GLfloat x, GLfloat y, GLfloat z);
    void setRotateMatrix(GLfloat angle, GLfloat ux, GLfloat uy, GLfloat uz);
    void setScaleMatrix(GLfloat sx, GLfloat sy, GLfloat sz);
    void setFrustumMatrix(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar);
    void doTranslate(GLfloat x, GLfloat y, GLfloat z);
    void doRotate(GLfloat angle, GLfloat ux, GLfloat uy, GLfloat uz);
    void doArbitraryRotate(GLfloat angle, Vector3 v1, Vector3 v2);
    void doScale(GLfloat scale);
    void doScale(GLfloat sx, GLfloat sy, GLfloat sz);
    void doFrustum(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar);

private:
    GLfloat matrix[16];
};
