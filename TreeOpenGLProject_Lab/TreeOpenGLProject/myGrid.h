#pragma once
#include <vector>
#include "GL/freeglut.h"

class MyGrid
{
public:
    MyGrid();
    void Render2DGrid();
    void SetDimension(int dim);
    void SetFilledCell(int x, int y, bool filled);
    int GetGridDimension() const;
    void ResetFilledCells();
    void SetGridColor(GLfloat r, GLfloat g, GLfloat b);

private:
    int dimensions_;
    GLfloat cell_color_[3];
    std::vector<bool> filled_cells_;

    void CreateGrid();
    bool IsCellFilled(int x, int y);
    void DrawBox(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2) const;
};
