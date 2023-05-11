#include "myGrid.h"

MyGrid::MyGrid() :
    grid_size_x_(20.0f),
    grid_size_y_(20.0f),
    dimensions_(10),
    pixel_color_{1.0f, 0.2f, 1.0f}
{
    InitializePixelGrid();
}

void MyGrid::RenderGrid() const
{
    const int pixel_grid_size = GetPixelGridSize();

    // if dimensions=15, then size=31
    // do scale from (0)---(31) to (-10)---(10)
    glPushMatrix();
    glTranslatef(-grid_size_x_ / 2.0f, -grid_size_y_ / 2.0f, 0);
    glScalef(grid_size_x_ / static_cast<GLfloat>(pixel_grid_size), grid_size_y_ / static_cast<GLfloat>(pixel_grid_size), 1.0f);

    // draw grid
    glBegin(GL_LINES);
    glColor3f(1.0f, 1.0f, 1.0f);
    for (int i = 0; i <= pixel_grid_size; ++i)
    {
        // vertical line
        glVertex2i(i, 0);
        glVertex2i(i, pixel_grid_size);
        // horizontal line
        glVertex2i(0, i);
        glVertex2i(pixel_grid_size, i);
    }
    glEnd();

    // draw pixel
    glBegin(GL_QUADS);
    for (int x = 0; x < pixel_grid_size; ++x)
    {
        for (int y = 0; y < pixel_grid_size; ++y)
        {
            const bool is_filled = pixel_grid_.at(x + y * pixel_grid_size);
            if (is_filled)
            {
                glColor3f(pixel_color_[0], pixel_color_[1], pixel_color_[2]);
                glVertex2i(x, y);
                glVertex2i(x, y + 1);
                glVertex2i(x + 1, y + 1);
                glVertex2i(x + 1, y);
            }
        }
    }
    glEnd();

    glPopMatrix();
}

void MyGrid::SetDimension(const int dim)
{
    dimensions_ = dim;
    InitializePixelGrid();
}

void MyGrid::SetPixel(int x, int y, const bool is_filled)
{
    const int pixel_grid_size = GetPixelGridSize();
    x += dimensions_;
    y += dimensions_;
    pixel_grid_.at(x + y * pixel_grid_size) = is_filled;
}

void MyGrid::SetPixelColor(const Color& color)
{
    pixel_color_ = color;
}

int MyGrid::GetGridDimension() const
{
    return dimensions_;
}

int MyGrid::GetPixelGridSize() const
{
    return dimensions_ * 2 + 1;
}

void MyGrid::RemoveAllPixel()
{
    InitializePixelGrid();
}

void MyGrid::InitializePixelGrid()
{
    const int pixel_grid_size = GetPixelGridSize();
    pixel_grid_ = std::vector<bool>(pixel_grid_size * pixel_grid_size);
    for (auto pixel : pixel_grid_)
    {
        pixel = false;
    }
}
