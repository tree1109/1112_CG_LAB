#pragma once
#include <vector>
#include "GL\freeglut.h"

class my2dGrid
{
public:
    my2dGrid();
    void render2DGrid();
    void setDimension(int dim);
    void setFilledCell(int x, int y, bool filled);
    int getGridDimension();
    void resetFilledCells();
    void setGridColor(GLfloat r, GLfloat g, GLfloat b);

private:
    int _dimensions;
    GLfloat _cellColor[3];
    std::vector<bool> _filledCells;

    void createGrid();
    bool isCellFilled(int x, int y);
    void drawBox(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2);
};
