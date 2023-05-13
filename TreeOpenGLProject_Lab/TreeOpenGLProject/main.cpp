#include <iostream>
#include <iomanip>
#include <array>
#include "GL\freeglut.h" // freeglut
#include "main.h"
#include "myPopupMenu.h"
#include "myGrid.h"

// ~~~key map~~~
// [r] : clean up grid and vertex
// ~~~key map~~~

MyGrid vertexGrid;
MyGrid lineGreenGrid; // for E
MyGrid lineBlueGrid;  // for NE

std::array<int, 2> v1 = {3,3};
std::array<int, 2> v2 = {3,-3};
std::array<int, 2> v3 = {-3,-3};
std::array<int, 2> v4 = {-3,3};

// Normal mode 顯示所有點下去紅點
// Debug mode 顯示連起來的線
bool onDebugMode = false;

// FSM for setting vertex
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
    glutCreateWindow("this is my grid");
    SetupRC();

    // Register callbacks for GLUT
    glutReshapeFunc(ChangeSize);
    glutDisplayFunc(RenderScene);
    glutKeyboardFunc(myKeyboard);
    glutMouseFunc(myMouse);

    myPopupMenu::CreatePopupMenu();

    // set grid color
    vertexGrid.SetPixelColor({1.0f, 0.5f, 0.5f});
    lineGreenGrid.SetPixelColor({ 0.5f, 1.0f, 0.5f });
    lineBlueGrid.SetPixelColor({0.5f, 0.5f, 1.0f});

    glutMainLoop();
    return 0;
}

void ChangeSize(const int w, const int h)
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
    gluLookAt(0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    vertexGrid.RenderGrid();
    if (onDebugMode) {
        lineGreenGrid.RenderGrid();
        lineBlueGrid.RenderGrid();
    }

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

void myKeyboard(const unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'r':
        // clean up grid and vertex
        vertexGrid.RemoveAllPixel();
        lineGreenGrid.RemoveAllPixel();
        lineBlueGrid.RemoveAllPixel();
        currentVertex = CURRENT_VERTEX::V1;
        break;
    default:
        break;
    }
    glutPostRedisplay();
}

void myMouse(const int button, const int state, const int x, const int y)
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
    GLfloat translateToGrid = vertexGrid.GetGridDimension() + halfOfCellSize;
    int gridX = static_cast<int>(round(clipX * translateToGrid));
    int gridY = static_cast<int>(round(clipY * translateToGrid));

    switch (button)
    {
    case GLUT_LEFT_BUTTON:
        if (state == GLUT_DOWN) {
            //printMouseWindowCoordinate(gridX, gridY, true);
            setVertex(gridX, gridY);
            vertexGrid.SetPixel(gridX, gridY, true);
        } else if (state == GLUT_UP) {
            //printMouseWindowCoordinate(gridX, gridY, false);
        }
        break;
    default:
        break;
    }
    glutPostRedisplay();
}

void setVertex(const int x, const int y)
{
    switch (currentVertex)
    {
    case CURRENT_VERTEX::V1:
        vertexGrid.RemoveAllPixel();
        lineGreenGrid.RemoveAllPixel();
        lineBlueGrid.RemoveAllPixel();
        v1 = { x, y };
        printVertexPixelCoordinate("v1", v1);
        currentVertex = CURRENT_VERTEX::V2;
        break;
    case CURRENT_VERTEX::V2:
        v2 = { x, y };
        printVertexPixelCoordinate("v2", v2);
        linePainter(v1, v2);
        if (onDebugMode) printLineRegion("v1v2", v1, v2);
        currentVertex = CURRENT_VERTEX::V3;
        break;
    case CURRENT_VERTEX::V3:
        v3 = { x, y };
        printVertexPixelCoordinate("v3", v3);
        linePainter(v2, v3);
        if (onDebugMode) printLineRegion("v2v3", v2, v3);
        currentVertex = CURRENT_VERTEX::V4;
        break;
    case CURRENT_VERTEX::V4:
        v4 = { x, y };
        printVertexPixelCoordinate("v4", v4);
        linePainter(v3, v4);
        if (onDebugMode) printLineRegion("v3v4", v3, v4);
        linePainter(v4, v1);
        if (onDebugMode) printLineRegion("v4v1", v4, v1);
        currentVertex = CURRENT_VERTEX::V1;
        break;
    }
}

void linePainter(const std::array<int, 2> v1, const std::array<int, 2> v2)
{
    // line color:
    //    endpoint: red
    //    line: green(E) or blue(NE)
    midpointAlgorithm(v1[0], v1[1], v2[0], v2[1]);
}

int getRegion(const std::array<int, 2> v1, const std::array<int, 2> v2)
{
    const bool steep = abs(v2[1] - v1[1]) > abs(v2[0] - v1[0]); // is m > 1?
    const bool isLeft2Right = v1[0] < v2[0];
    const bool isBottom2Top = v1[1] < v2[1];

    if (isLeft2Right) {
        // I
        if (isBottom2Top)
            return steep ? 2 : 1;
        // IV
        return steep ? 7 : 8;
    }
    // II
    if (isBottom2Top)
        return steep ? 3 : 4;
    // III
    return steep ? 6 : 5;
}

// Bresenham Algorithm
void midpointAlgorithm(int x1, int y1, int x2, int y2)
{
    // is m > 1 ?
    // translate with y=x to make m <= 1
    const bool steep = abs(y2 - y1) > abs(x2 - x1);
    if (steep) {
        std::swap(x1, y1);
        std::swap(x2, y2);
    }
    // is right to left ?
    // always draw line from left to right
    if (x1 > x2) {
        std::swap(x1, x2);
        std::swap(y1, y2);
    }

    const int dx = x2 - x1;
    const int dy = y2 - y1;
    const int deltaE = 2 * dy;         // 2* (a)
    const int deltaNE = 2 * (dy - dx); // 2* (a+b) for m >= 0
    const int deltaSE = 2 * (dy + dx); // 2* (a-b) for m < 0
    int decision;
    int x = x1;
    int y = y1;

    // draw E or NE(SE) pixel according to steep
    auto drawPixel = [&](const bool isE) {
        if (steep) {
            if (isE) lineGreenGrid.SetPixel(y, x, true);
            else lineBlueGrid.SetPixel(y, x, true);
            if (onDebugMode) printLinePixelCoordinate({ y, x }, true);
        }
        else {
            if (isE) lineGreenGrid.SetPixel(x, y, true);
            else lineBlueGrid.SetPixel(x, y, true);
            if (onDebugMode) printLinePixelCoordinate({ x, y }, true);
        }
    };

    // draw first pixel
    drawPixel(true);

    // top to bottom
    if (y1 > y2) {
        decision = 2 * dy + dx; // 2* (a-b/2) for m < 0
        while (x < x2) {
            if (decision >= 0) {
                // E
                decision += deltaE;
                ++x;
                drawPixel(true);
            }
            else {
                // SE
                decision += deltaSE;
                ++x;
                --y;
                drawPixel(false);
            }
        }
    }
    // bottom to top
    else {
        decision = 2 * dy - dx; // 2* (a+b/2) for m >= 0
        while (x < x2) {
            if (decision <= 0) {
                // E
                decision += deltaE;
                ++x;
                drawPixel(true);
            }
            else {
                // NE
                decision += deltaNE;
                ++x;
                ++y;
                drawPixel(false);
            }
        }
    }
}

// message printer
void printVertexPixelCoordinate(const std::string& name, const std::array<int, 2>& vertex)
{
    std::cout << "[info] Vertex \033[91m" << name << "\033[0m at \033[91m(" << vertex[0] << ", " << vertex[1] << ")\033[0m" << std::endl;
}

void printLinePixelCoordinate(const std::array<int, 2>& vertex, const bool isE)
{
    if (isE)
        std::cout << "\033[93m[debug]\033[0m Line pixel \033[92mE\033[0m at \033[92m(" << vertex[0] << ", " << vertex[1] << ")\033[0m" << std::endl;
    else
        std::cout << "\033[93m[debug]\033[0m Line pixel \033[94mNE\033[0m at \033[94m(" << vertex[0] << ", " << vertex[1] << ")\033[0m" << std::endl;
}

void printLineRegion(const std::string& name, const std::array<int, 2>& v1, const std::array<int, 2>& v2)
{
    std::cout << "\033[93m[debug]\033[0m Line \033[96m" << name << "\033[0m is \033[96mregion " << getRegion(v1, v2) << "\033[0m" << std::endl;
}

// for popup menu
void setGridDimension(const int dim)
{
    vertexGrid.SetDimension(dim);
    lineGreenGrid.SetDimension(dim);
    lineBlueGrid.SetDimension(dim);
    currentVertex = CURRENT_VERTEX::V1;
    glutPostRedisplay();
}

void switchDebugMode()
{
    onDebugMode = !onDebugMode;
    if (onDebugMode)
        std::cout << "[info] Switch to \033[93mDebug Mode\033[0m" << std::endl;
    else
        std::cout << "[info] Switch to \033[92mNormal Mode\033[0m" << std::endl;
    glutPostRedisplay();
}
