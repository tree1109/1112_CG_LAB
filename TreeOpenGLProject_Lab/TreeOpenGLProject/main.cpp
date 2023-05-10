#include <iostream>
#include <iomanip>
#include "GL\freeglut.h" // freeglut
#include "main.h"
#include "myPopupMenu.h"
#include "my2dGrid.h"

// ~~~key map~~~
// [r] : reset
// 
// ~~~key map~~~

my2dGrid vertexGrid;
my2dGrid lineGrid;

int v1[] = {3,3};
int v2[] = {3,-3};
int v3[] = {-3,-3};
int v4[] = {-3,3};

enum class CURRENT_VERTEX
{
    V1,
    V2,
    V3,
    V4
};
CURRENT_VERTEX currentVertex = CURRENT_VERTEX::V1;

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(400, 400);
    glutInitWindowPosition(600, 80);
    glutCreateWindow("model viewer!?");
    SetupRC();

    // Register callbacks for GLUT
    glutReshapeFunc(ChangeSize);
    glutDisplayFunc(RenderScene);
    glutKeyboardFunc(myKeyboard);
    glutMouseFunc(myMouse);
    glutMotionFunc(myMotion);

    myPopupMenu::CreatePopupMenu();

    // set grid color
    vertexGrid.setGridColor(1.0f, 0.2f, 0.2f);
    lineGrid.setGridColor(0.2f, 1.0f, 0.2f);

    glutMainLoop();
    return 0;
}

void ChangeSize(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION); // load the projection matrix
    glLoadIdentity();
    glOrtho(-10, 10, -10, 10, -10, 20);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void RenderScene()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW); // load the model view matrix
    glLoadIdentity(); // reset model matrix
    gluLookAt(0.0f, 0.0f, 10.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

    lineGrid.render2DGrid();
    vertexGrid.render2DGrid();

    glutSwapBuffers();
}

void SetupRC()
{
    // Light values and coordinates
    GLfloat whiteLight[] = { 0.45f, 0.45f, 0.45f, 1.0f };
    GLfloat sourceLight[] = { 0.25f, 0.25f, 0.25f, 1.0f };
    GLfloat lightPos[] = { 0.f, 25.0f, 20.0f, 0.0f };
    // Enable lighting
    glEnable(GL_LIGHTING);
    // Setup and enable light 0
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, whiteLight);
    glLightfv(GL_LIGHT0, GL_AMBIENT, sourceLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, sourceLight);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glEnable(GL_LIGHT0);
    // Enable color tracking
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_DEPTH_TEST);
}

void myKeyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'r':
        // reset
        // TODO: clear grid filled cell
        break;
    default:
        break;
    }
    glutPostRedisplay();
}

void myMouse(int button, int state, int x, int y)
{
    // screen space:
    // o---→ x
    // |
    // ↓ y
    // 
    // world space:
    // ↑ y
    // |
    // o--→ x
    // 
    // more at: https://learnopengl.com/Getting-started/Coordinate-Systems
    // 
    // transform screen coordinate to world coordinate
    GLfloat clipX = static_cast<GLfloat>(x) / glutGet(GLUT_WINDOW_WIDTH) * 2 - 1;
    GLfloat clipY = (1 - static_cast<GLfloat>(y) / glutGet(GLUT_WINDOW_HEIGHT)) * 2 - 1;
    GLfloat halfOfCellSize = 0.5f;
    GLfloat translateToGrid = vertexGrid.getGridDimension() + halfOfCellSize;
    int gridX = static_cast<int>(round(clipX * translateToGrid));
    int gridY = static_cast<int>(round(clipY * translateToGrid));

    switch (button)
    {
    case GLUT_LEFT_BUTTON:
        if (state == GLUT_DOWN) {
            setVertex(gridX, gridY);
            vertexGrid.setFilledCell(gridX, gridY, true);
            printMouseWindowCoordinate(gridX, gridY, true);
        } else if (state == GLUT_UP) {
            printMouseWindowCoordinate(gridX, gridY, false);
        }
        break;
    default:
        break;
    }
    glutPostRedisplay();
}

void myMotion(int x, int y)
{
    //glutPostRedisplay();
}

void printMouseWindowCoordinate(int x, int y, bool isDown) {
    if (isDown) {
        std::cout << "[info] mouse \033[93mdown\033[0m at (" << x << ", " << y << ")" << std::endl;
    } else {
        std::cout << "[info] mouse \033[92mup\033[0m at (" << x << ", " << y << ")" << std::endl;
    }
}

void setVertex(int x, int y) {
    switch (currentVertex)
    {
    case CURRENT_VERTEX::V1:
        vertexGrid.resetFilledCells();
        lineGrid.resetFilledCells();
        v1[0] = x;
        v1[1] = y;
        currentVertex = CURRENT_VERTEX::V2;
    break;
    case CURRENT_VERTEX::V2:
        v2[0] = x;
        v2[1] = y;
        currentVertex = CURRENT_VERTEX::V3;
        linePainter(v1[0], v1[1], v2[0], v2[1]);
    break;
    case CURRENT_VERTEX::V3:
        v3[0] = x;
        v3[1] = y;
        currentVertex = CURRENT_VERTEX::V4;
        linePainter(v2[0], v2[1], v3[0], v3[1]);
        break;
    case CURRENT_VERTEX::V4:
        v4[0] = x;
        v4[1] = y;
        currentVertex = CURRENT_VERTEX::V1;
        linePainter(v3[0], v3[1], v4[0], v4[1]);
        linePainter(v4[0], v4[1], v1[0], v1[1]);
        break;
    }
}

void linePainter(int x1, int y1, int x2, int y2) {
    // color:
    //    endpoint: red
    //    line: green or blue
    int region = getRegion(x1, y1, x2, y2);
    if (region == 0)
        std::cout << "[error] Can't get region." << std::endl;

    std::cout << "[info] region: " << region << std::endl;

    // TODO: use Midpoint algorithm draw line
    int dx = x2 - x1;
    int dy = y2 - y1;
    int d = 2 * dy - dx;
    int deltaE = 2 * dy;
    int deltaNE = 2 * (dy - dx);
    int x = x1;
    int y = y1;
    lineGrid.setFilledCell(x, y, true);
    while (x < x2) {
        if (d <= 0) {
            d += deltaE;
            x = x + 1;
        } else {
            d += deltaNE;
            x = x + 1;
            y = y + 1;
        }
        lineGrid.setFilledCell(x, y, true);
    }
}

int getRegion(int x1, int y1, int x2, int y2) {
    int deltaX = x2 - x1;
    int deltaY = y2 - y1;
    if (deltaX >= 0 && deltaY >= 0) {
        if (deltaX >= deltaY)
            return 1;
        return 2;
    }
    if (deltaX < 0 && deltaY >= 0) {
        if (abs(deltaX) >= deltaY)
            return 4;
        return 3;
    }
    if (deltaX < 0 && deltaY < 0) {
        if (abs(deltaX) >= abs(deltaY))
            return 5;
        return 6;
    }
    if (deltaX >= 0 && deltaY < 0) {
        if (deltaX >= abs(deltaY))
            return 8;
        return 7;
    }

    return 0;
}

void setGridDimension(int dim) {
    vertexGrid.setDimension(dim);
    lineGrid.setDimension(dim);
    glutPostRedisplay();
}
