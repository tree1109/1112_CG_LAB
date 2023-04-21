#pragma once
#include "vec3.h"

class myMatrix
{
public:
    myMatrix(void);
    void ResetMatrix(void);
    myMatrix Mult(const myMatrix &rightM) const;
    void setTranslateMatrix(GLfloat x, GLfloat y, GLfloat z);
    void setRotateMatrix(GLfloat angle, GLfloat ux, GLfloat uy, GLfloat uz);
    void setScaleMatrix(GLfloat sx, GLfloat sy, GLfloat sz);
    void doTranslate(GLfloat x, GLfloat y, GLfloat z);
    void doRotate(GLfloat angle, GLfloat ux, GLfloat uy, GLfloat uz);
    void doArbitraryRotate(GLfloat angle, vec3 v1, vec3 v2);
    void doScale(GLfloat scale);
    void doScale(GLfloat sx, GLfloat sy, GLfloat sz);

private:
    GLfloat matrix[16];
};
