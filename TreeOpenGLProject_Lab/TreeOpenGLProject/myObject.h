#pragma once
#include <string>
#include <vector>
#include "GL\freeglut.h" // freeglut

struct vec3
{
    GLfloat x;
    GLfloat y;
    GLfloat z;
};
struct face3
{
    int v1;
    int v2;
    int v3;
};
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
    void drawObject();
    void loadObjectFile(std::string filename);
    void setRenderMode(RENDER_MODE renderMode);
    void setColorMode(COLOR_MODE colorMode);
    void setColor(GLfloat r, GLfloat g, GLfloat b);

private:
    std::vector<vec3> _vertices;
    std::vector<face3> _faces;
    vec3 _color;
    RENDER_MODE _renderMode;
    COLOR_MODE _colorMode;
    GLfloat _pointSize;
    GLfloat _lineWidth;

    void drawPoints();
    void drawLines();
    void drawFaces();
    void fillColor();
};

