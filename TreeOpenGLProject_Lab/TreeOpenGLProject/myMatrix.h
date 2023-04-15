#pragma once
class myMatrix
{
    public:
    myMatrix(void);
    void ResetMatrix(void);
    myMatrix Mult(const myMatrix &rightM) const;
    void setTranslateMatrix(GLfloat x, GLfloat y, GLfloat z);
    void setRotateMatrix(GLfloat angle, GLfloat ux, GLfloat uy, GLfloat uz);
    void doTranslate(GLfloat x, GLfloat y, GLfloat z);
    void doRotate(GLfloat angle, GLfloat ux, GLfloat uy, GLfloat uz);
    void doArbitraryRotate(GLfloat angle, GLfloat p1[], GLfloat p2[]);

private:
    GLfloat matrix[16];
};
