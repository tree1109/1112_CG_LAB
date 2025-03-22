#pragma once
#include <string>
#include <vector>
#include "GL\freeglut.h" // freeglut
#include "myMatrix.h"
#include "face3.h"

#include "Math.h"

enum class RENDER_MODE
{
    POINTS,
    LINES,
    FACES
};
enum class COLOR_MODE
{
    SINGLE,
    RANDOM
};

class myObject
{
public:
    myObject();
    void drawObject(bool showBoundingBox = false);
    void loadObjectFile(std::string filename);
    void doTransformation();
    void setTransformation(Vector3 position, Vector3 rotation, GLfloat scaling);
    void setRenderMode(RENDER_MODE renderMode);
    void setColorMode(COLOR_MODE colorMode);
    void setColor(GLfloat r, GLfloat g, GLfloat b);
    GLfloat getScalingCoefficient();
    void fitToWindow();
    void setArbitraryRotate(GLfloat theta, Vector3 v1, Vector3 v2);

private:
    std::vector<Vector3> _vertices;
    std::vector<face3> _faces;
    Vector3 _color;
    RENDER_MODE _renderMode;
    COLOR_MODE _colorMode;
    GLfloat _pointSize;
    GLfloat _lineWidth;

    // transformation data
    Vector3 _position;
    Vector3 _rotation;
    GLfloat _scaling;
    GLfloat _arbitraryTheta;
    Vector3 _v1;
    Vector3 _v2;
    myMatrix _transformationMatrix;
    
    struct BoundingBox
    {
        GLfloat xMin;
        GLfloat xMax;
        GLfloat yMin;
        GLfloat yMax;
        GLfloat zMin;
        GLfloat zMax;
    };
    BoundingBox _boundingBox;

    void drawPoints();
    void drawLines();
    void drawFaces();
    void fillColor();
    void createBoundingBox();
    void drawBoundingBox();
    void drawArbitraryAxis();
    void drawCross(Vector3 v);
    void fixPositionVector();
    Vector3 getNormalVector(Vector3 v1, Vector3 v2, Vector3 v3);
};
