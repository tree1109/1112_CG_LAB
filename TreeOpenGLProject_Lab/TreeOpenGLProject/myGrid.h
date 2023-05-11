#pragma once
#include <vector>
#include <array>
#include "GL/freeglut.h"

typedef std::array<GLfloat, 3> Color;

class MyGrid
{
public:
    MyGrid();
    void RenderGrid() const;
    void SetDimension(int dim);
    void SetPixel(int x, int y, bool is_filled);
    void SetPixelColor(const Color& color);
    int GetGridDimension() const;
    int GetPixelGridSize() const;
    void RemoveAllPixel();

private:
    GLfloat grid_size_x_;
    GLfloat grid_size_y_;
    int dimensions_;
    Color pixel_color_;
    std::vector<bool> pixel_grid_;

    void InitializePixelGrid();
};
