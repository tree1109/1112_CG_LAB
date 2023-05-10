#include "myGrid.h"

MyGrid::MyGrid() :
    dimensions_(10),
    cell_color_{ 1.0f, 0.0f, 1.0f }
{
    CreateGrid();
}

void MyGrid::Render2DGrid()
{
    const int size = dimensions_ * 2 + 1;
    const GLfloat delta_x = 20.0f / static_cast<GLfloat>(size);
    const GLfloat delta_y = 20.0f / static_cast<GLfloat>(size);
    GLfloat range_x = delta_x / 2;
    GLfloat range_y = delta_y / 2;
    for (int i = 0; i <= dimensions_; ++i) {
        glBegin(GL_LINES);
        glColor3f(1, 1, 1);
        // row
        glVertex3f(-10, range_y, 0);
        glVertex3f(10, range_y, 0);
        glVertex3f(-10, -range_y, 0);
        glVertex3f(10, -range_y, 0);
        // col
        glVertex3f(range_x, 10, 0);
        glVertex3f(range_x, -10, 0);
        glVertex3f(-range_x, 10, 0);
        glVertex3f(-range_x, -10, 0);
        glEnd();
        range_x += delta_x;
        range_y += delta_y;
    }

    for (int i = -dimensions_; i <= dimensions_; ++i) {
        for (int j = -dimensions_; j <= dimensions_; ++j) {
            const GLfloat left = static_cast<GLfloat>(i) * delta_x - delta_x / 2;
            const GLfloat right = static_cast<GLfloat>(i) * delta_x + delta_x / 2;
            const GLfloat up = static_cast<GLfloat>(j) * delta_y - delta_y / 2;
            const GLfloat down = static_cast<GLfloat>(j) * delta_y + delta_y / 2;
            if (IsCellFilled(i, j))
                DrawBox(left, up, right, down);
        }
    }
}

void MyGrid::SetDimension(const int dim)
{
    dimensions_ = dim;
    CreateGrid();
}

void MyGrid::SetFilledCell(const int x, const int y, const bool filled)
{
    const int grid_x = x + dimensions_;
    const int grid_y = y + dimensions_;
    const int cells_per_row = dimensions_ * 2 + 1;
    filled_cells_.at(grid_x + grid_y * cells_per_row) = filled;
}

int MyGrid::GetGridDimension() const
{
    return dimensions_;
}

void MyGrid::ResetFilledCells()
{
    CreateGrid();
}

void MyGrid::SetGridColor(const GLfloat r, const GLfloat g, const GLfloat b)
{
    cell_color_[0] = r;
    cell_color_[1] = g;
    cell_color_[2] = b;
}

void MyGrid::CreateGrid()
{
    const int size = dimensions_ * 2 + 1;
    const auto cells = std::vector<bool>(size * size);
    filled_cells_ = cells;
}

bool MyGrid::IsCellFilled(const int x, const int y)
{
    const int grid_x = x + dimensions_;
    const int grid_y = y + dimensions_;
    const int cells_per_row = dimensions_ * 2 + 1;
    return filled_cells_.at(grid_x + grid_y * cells_per_row);
}

void MyGrid::DrawBox(const GLfloat x1, const GLfloat y1, const GLfloat x2, const GLfloat y2) const
{
    glBegin(GL_QUADS);
    glColor3f(cell_color_[0], cell_color_[1], cell_color_[2]);
    glVertex3f(x1, y1, 0.0f);
    glVertex3f(x2, y1, 0.0f);
    glVertex3f(x2, y2, 0.0f);
    glVertex3f(x1, y2, 0.0f);
    glEnd();
}
