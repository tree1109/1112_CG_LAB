#include <iostream>
#include <iomanip>
#include <array>
#include "GL\freeglut.h" // freeglut
#include "main.h"
#include "myPopupMenu.h"
#include "myGrid.h"

// ~~~key map~~~
// [r] : reset
// 
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
    vertexGrid.SetPixelColor({1.0f, 0.5f, 0.5f});
    lineGreenGrid.SetPixelColor({ 0.5f, 1.0f, 0.5f });
    lineBlueGrid.SetPixelColor({0.5f, 0.5f, 1.0f});

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

void myKeyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'r':
        // clean all pixels
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
    GLfloat translateToGrid = vertexGrid.GetGridDimension() + halfOfCellSize;
    int gridX = static_cast<int>(round(clipX * translateToGrid));
    int gridY = static_cast<int>(round(clipY * translateToGrid));

    switch (button)
    {
    case GLUT_LEFT_BUTTON:
        if (state == GLUT_DOWN) {
            printMouseWindowCoordinate(gridX, gridY, true);
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
        vertexGrid.RemoveAllPixel();
        lineGreenGrid.RemoveAllPixel();
        lineBlueGrid.RemoveAllPixel();
        v1 = { x, y };
        currentVertex = CURRENT_VERTEX::V2;
        break;
    case CURRENT_VERTEX::V2:
        v2 = { x, y };
        currentVertex = CURRENT_VERTEX::V3;
        linePainter(v1, v2);
        break;
    case CURRENT_VERTEX::V3:
        v3 = { x, y };
        currentVertex = CURRENT_VERTEX::V4;
        linePainter(v2, v3);
        break;
    case CURRENT_VERTEX::V4:
        v4 = { x, y };
        currentVertex = CURRENT_VERTEX::V1;
        linePainter(v3, v4);
        linePainter(v4, v1);
        std::cout << "[info] Line \033[96mv1v2\033[0m region: " << getRegion(v1, v2) << std::endl;
        std::cout << "[info] Line \033[96mv2v3\033[0m region: " << getRegion(v2, v3) << std::endl;
        std::cout << "[info] Line \033[96mv3v4\033[0m region: " << getRegion(v3, v4) << std::endl;
        std::cout << "[info] Line \033[96mv4v1\033[0m region: " << getRegion(v4, v1) << std::endl;
        break;
    }
}

void linePainter(std::array<int, 2> v1, std::array<int, 2> v2) {
    // color:
    //    endpoint: red
    //    line: green(E) or blue(NE)
    midpointAlgorithm(v1[0], v1[1], v2[0], v2[1], onDebugMode);
}

int getRegion(std::array<int, 2> v1, std::array<int, 2> v2) {
    int deltaX = v2[0] - v1[0];
    int deltaY = v2[1] - v1[1];
    if (deltaX >= 0 && deltaY >= 0) {
        if (deltaX >= deltaY)
            return 1;
        return 2;
    }
    if (deltaX < 0 && deltaY >= 0) {
        if (-deltaX >= deltaY)
            return 4;
        return 3;
    }
    if (deltaX < 0 && deltaY < 0) {
        if (-deltaX >= -deltaY)
            return 5;
        return 6;
    }
    if (deltaX >= 0 && deltaY < 0) {
        if (deltaX >= -deltaY)
            return 8;
        return 7;
    }

    return 0;
}

// Bresenham Algorithm
void midpointAlgorithm(int x1, int y1, int x2, int y2, bool printCoordinate)
{
    const bool steep = abs(y2 - y1) > abs(x2 - x1);
    if (steep) {
        std::swap(x1, y1);
        std::swap(x2, y2);
    }
    if (x1 > x2) {
        std::swap(x1, x2);
        std::swap(y1, y2);
    }
    const int dx = x2 - x1;
    const int dy = y2 - y1;
    const int delta_e = 2 * dy;         // 2* (a)
    const int delta_ne = 2 * (dy - dx); // 2* (a+b) for m >= 0
    const int delta_se = 2 * (dy + dx); // 2* (a-b) for m < 0
    int decision;

    auto printVertex = [&printCoordinate](int x, int y) {
        if (printCoordinate)
            std::cout << "[debug] Line pixel: (" << x << ", " << y << ")" << std::endl;
    };

    int x = x1;
    int y = y1;
    if (steep) {
        lineGreenGrid.SetPixel(y, x, true);
        printVertex(y, x);
    }
    else {
        lineGreenGrid.SetPixel(x, y, true);
        printVertex(x, y);
    }

    if (y1>y2) {
        decision = 2 * dy + dx; // 2* (a-b/2) for m < 0
        while (x < x2) {
            if (decision > 0) {
                decision += delta_e;
                ++x;
                if (steep) {
                    lineGreenGrid.SetPixel(y, x, true);
                    printVertex(y, x);
                }
                else {
                    lineGreenGrid.SetPixel(x, y, true);
                    printVertex(x, y);
                }
            }
            else {
                decision += delta_se;
                ++x;
                --y;
                if (steep) {
                    lineBlueGrid.SetPixel(y, x, true);
                    printVertex(y, x);
                }
                else {
                    lineBlueGrid.SetPixel(x, y, true);
                    printVertex(x, y);
                }
            }
        }
    }
    else {
        decision = 2 * dy - dx; // 2* (a+b/2) for m >= 0
        while (x < x2) {
            if (decision <= 0) {
                decision += delta_e;
                ++x;
                if (steep) {
                    lineGreenGrid.SetPixel(y, x, true);
                    printVertex(y, x);
                }
                else {
                    lineGreenGrid.SetPixel(x, y, true);
                    printVertex(x, y);
                }
            }
            else {
                decision += delta_ne;
                ++x;
                ++y;
                if (steep) {
                    lineBlueGrid.SetPixel(y, x, true);
                    printVertex(y, x);
                }
                else {
                    lineBlueGrid.SetPixel(x, y, true);
                    printVertex(x, y);
                }
            }
        }
    }
}

void setGridDimension(int dim) {
    vertexGrid.SetDimension(dim);
    lineGreenGrid.SetDimension(dim);
    lineBlueGrid.SetDimension(dim);
    currentVertex = CURRENT_VERTEX::V1;
    glutPostRedisplay();
}

void setGridDebugMode(bool isOn)
{
    onDebugMode = isOn;
    glutPostRedisplay();
}
