#include "my2dGrid.h"

my2dGrid::my2dGrid() :
    _dimensions(10)
{
    creatGrid();
}

void my2dGrid::render2DGrid()
{
    int size = _dimensions * 2 + 1;
    GLfloat deltaX = 20.0f / size;
    GLfloat deltaY = 20.0f / size;
    GLfloat rangeX = deltaX / 2;
    GLfloat rangeY = deltaY / 2;
    for (int i = 0; i <= _dimensions; ++i) {
        glBegin(GL_LINES);
        glColor3f(1, 1, 1);
        // row
        glVertex3f(-10, rangeY, 0);
        glVertex3f(10, rangeY, 0);
        glVertex3f(-10, -rangeY, 0);
        glVertex3f(10, -rangeY, 0);
        // col
        glVertex3f(rangeX, 10, 0);
        glVertex3f(rangeX, -10, 0);
        glVertex3f(-rangeX, 10, 0);
        glVertex3f(-rangeX, -10, 0);
        glEnd();
        rangeX += deltaX;
        rangeY += deltaY;
    }

    rangeX += deltaX;
    rangeY += deltaY;

    for (int i = -_dimensions; i <= _dimensions; ++i) {
        for (int j = -_dimensions; j <= _dimensions; ++j) {
            // TODO: draw box
        }
    }
}

void my2dGrid::setDimension(int dim)
{
    _dimensions = dim;
    creatGrid();
}

void my2dGrid::creatGrid()
{
    int size = _dimensions * 2 + 1;
    std::vector<bool> cells = std::vector<bool>(size * size);
    _filledCells = cells;
}

bool my2dGrid::isCellFilled(int x, int y)
{
    int gridX = x + _dimensions;
    int gridY = y + _dimensions;
    return _filledCells.at(gridX + gridY * _dimensions);
}

void my2dGrid::drawBox(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2)
{
    glBegin(GL_QUADS);
    glColor3f(1.0f, 0.0f, 1.0f);
    glVertex3f(x1, y1, 0.0f);
    glVertex3f(x2, y1, 0.0f);
    glVertex3f(x2, y2, 0.0f);
    glVertex3f(x1, y2, 0.0f);
    glEnd();
}
