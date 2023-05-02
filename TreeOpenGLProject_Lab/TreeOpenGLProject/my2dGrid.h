#pragma once
#include <vector>
#include "GL\freeglut.h"

class my2dGrid
{
public:
    my2dGrid();
    void render2DGrid();
    void setDimension(int dim);

private:
    int _dimensions;
    std::vector<bool> _filledCells;

    void creatGrid();
    bool isCellFilled(int x, int y);
    void drawBox(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2);
};

