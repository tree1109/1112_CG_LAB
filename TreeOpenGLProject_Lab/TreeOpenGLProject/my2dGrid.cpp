#include "my2dGrid.h"

my2dGrid::my2dGrid() :
    _dimensions(10),
    _cellColor{ 1.0f, 0.0f, 1.0f }
{
    createGrid();
}

void my2dGrid::render2DGrid()
{
    int size = _dimensions * 2 + 1;
    const GLfloat deltaX = 20.0f / size;
    const GLfloat deltaY = 20.0f / size;
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

    for (int i = -_dimensions; i <= _dimensions; ++i) {
        for (int j = -_dimensions; j <= _dimensions; ++j) {
            GLfloat left = i * deltaX - deltaX / 2;
            GLfloat right = i * deltaX + deltaX / 2;
            GLfloat up = j * deltaY - deltaY / 2;
            GLfloat down = j * deltaY + deltaY / 2;
            if (isCellFilled(i, j))
                drawBox(left, up, right, down);
        }
    }
}

void my2dGrid::setDimension(int dim)
{
    _dimensions = dim;
    createGrid();
}

void my2dGrid::setFilledCell(int x, int y, bool filled)
{
    int gridX = x + _dimensions;
    int gridY = y + _dimensions;
    int cellsPerRow = _dimensions * 2 + 1;
    _filledCells.at(gridX + gridY * cellsPerRow) = filled;
}

int my2dGrid::getGridDimension()
{
    return _dimensions;
}

void my2dGrid::resetFilledCells()
{
    createGrid();
}

void my2dGrid::setGridColor(GLfloat r, GLfloat g, GLfloat b)
{
    _cellColor[0] = r;
    _cellColor[1] = g;
    _cellColor[2] = b;
}

void my2dGrid::createGrid()
{
    int size = _dimensions * 2 + 1;
    auto cells = std::vector<bool>(size * size);
    _filledCells = cells;
}

bool my2dGrid::isCellFilled(int x, int y)
{
    int gridX = x + _dimensions;
    int gridY = y + _dimensions;
    int cellsPerRow = _dimensions * 2 + 1;
    return _filledCells.at(gridX + gridY * cellsPerRow);
}

void my2dGrid::drawBox(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2)
{
    glBegin(GL_QUADS);
    glColor3f(_cellColor[0], _cellColor[1], _cellColor[2]);
    glVertex3f(x1, y1, 0.0f);
    glVertex3f(x2, y1, 0.0f);
    glVertex3f(x2, y2, 0.0f);
    glVertex3f(x1, y2, 0.0f);
    glEnd();
}
