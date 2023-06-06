#pragma once
#include <vector>
#include <array>
#include "GL/freeglut.h"

using Color = std::array<GLfloat, 3>;

class MyGrid
{
public:
    MyGrid();
    void RenderGrid() const;
    void SetDimension(int dim);
    void SetPixel(int x, int y, const Color& color);
    void SetVertexPixel(int x, int y);
    void SetEPixel(int x, int y);
    void SetNEPixel(int x, int y);
    void SetFacePixel(int x, int y);
    int GetGridDimension() const;
    int GetPixelGridSize() const;
    Color& GetPixelColorAt(int x, int y);
    bool isPixelColorFilledAt(int x, int y);
    void RemoveAllPixel();

private:
    GLfloat grid_size_x_;
    GLfloat grid_size_y_;
    int dimensions_;
    std::vector<Color> pixel_grid_;

    void InitializePixelGrid();
};
