#pragma once
class myMatrix
{
    public:
    myMatrix(void);
    ~myMatrix(void);
    void ResetMatrix(void);
    void TranslateMatrix(GLfloat x, GLfloat y, GLfloat z);
    void RotateMatrix(GLfloat angle, GLfloat ux, GLfloat uy, GLfloat uz);
    void ArbitraryRotate(GLfloat angle, GLfloat p1[], GLfloat p2[]);

private:
    // convert degree to radian
    const GLfloat deg2rad;
    GLfloat matrix[16];
};

