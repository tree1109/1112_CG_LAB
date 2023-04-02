#pragma once
using std::vector;

class myMatrix
{
    public:
    myMatrix(void);
    ~myMatrix(void);
    void ResetMatrix(void);
    void TranslateMatrix(GLfloat x, GLfloat y, GLfloat z);
    void RotateMatrix(GLfloat angle, GLfloat ux, GLfloat uy, GLfloat uz);
    void ArbitraryRotate(GLfloat angle, vector<GLfloat> p1, vector<GLfloat> p2);

private:
    // convert degree to radian
    const GLfloat deg2rad;
    vector<GLfloat> matrix;
};

