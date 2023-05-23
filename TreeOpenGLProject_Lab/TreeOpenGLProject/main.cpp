﻿#include <iostream>
#include <iomanip>
#include <array>
#include <vector>
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
MyGrid faceGrid;

Vertex v1 = {3,3};
Vertex v2 = {3,-3};
Vertex v3 = {-3,-3};

// Normal mode: only show vertex
// Debug mode: show vertex and line
bool onDebugMode = false;

// FSM for setting vertex
CURRENT_VERTEX currentVertex = CURRENT_VERTEX::V1;

// pixels needs to be rendered 
std::vector<Vertex> renderPixel = {};

// animation
const int TIME_INTERVAL = 13;
bool isRendering = false;

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
    faceGrid.SetPixelColor({ 1.0f,1.0f,0.5f });

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
        faceGrid.RenderGrid();
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
        faceGrid.RemoveAllPixel();
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
    const GLfloat clipX = static_cast<GLfloat>(x) / glutGet(GLUT_WINDOW_WIDTH) * 2 - 1;
    const GLfloat clipY = (1 - static_cast<GLfloat>(y) / glutGet(GLUT_WINDOW_HEIGHT)) * 2 - 1;
    const GLfloat halfOfCellSize = 0.5f;
    const GLfloat translateToGrid = static_cast<GLfloat>(vertexGrid.GetGridDimension()) + halfOfCellSize;
    const int gridX = static_cast<int>(round(clipX * translateToGrid));
    const int gridY = static_cast<int>(round(clipY * translateToGrid));
    switch (button)
    {
    case GLUT_LEFT_BUTTON:
        if (state == GLUT_DOWN) {
            setVertex(gridX, gridY);
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
        faceGrid.RemoveAllPixel();
        v1 = { x, y };
        vertexPainter(v1, "v1");
        currentVertex = CURRENT_VERTEX::V2;
        break;
    case CURRENT_VERTEX::V2:
        v2 = { x, y };
        vertexPainter(v2, "v2");
        linePainter(v1, v2, "v1v2");
        currentVertex = CURRENT_VERTEX::V3;
        break;
    case CURRENT_VERTEX::V3:
        v3 = { x, y };
        vertexPainter(v3, "v3");
        linePainter(v2, v3, "v2v3");
        linePainter(v3, v1, "v3v1");
        facePainter(v1, v2, v3, "v1v2v3");
        currentVertex = CURRENT_VERTEX::V1;
        break;
    }
}

void vertexPainter(const Vertex& v, const std::string& name)
{
    // vertex color: red
    vertexGrid.SetPixel(v[0], v[1], true);
    printVertexPixelCoordinate(v, name);
}

void linePainter(const Vertex& v1, const Vertex& v2, const std::string& name)
{
    // line color: green(E) or blue(NE)
    midpointAlgorithm(v1[0], v1[1], v2[0], v2[1]);
    if (onDebugMode) printLineRegion(v1, v2, name);
}

void facePainter(const Vertex& v1, const Vertex& v2, const Vertex& v3, const std::string& name)
{
    halfSpaceTest(v1, v2, v3);
    if (!isRendering) 
    {
        // start rendering
        isRendering = true;
        glutTimerFunc(TIME_INTERVAL, myTimer, 0);
    }
    else
    {
        std::cout << "\033[91m[error]\033[0m Pixels are rendering!!" << std::endl;
    }
}

int getRegion(const Vertex& v1, const Vertex& v2)
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
            if (onDebugMode) printLinePixelCoordinate({ y, x }, isE);
        }
        else {
            if (isE) lineGreenGrid.SetPixel(x, y, true);
            else lineBlueGrid.SetPixel(x, y, true);
            if (onDebugMode) printLinePixelCoordinate({ x, y }, isE);
        }
    };

    // top to bottom
    if (y1 > y2) {
        decision = 2 * dy + dx; // 2* (a-b/2) for m < 0
        while (x < x2 - 1) {
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
        while (x < x2 - 1) {
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

void halfSpaceTest(const Vertex& v1, const Vertex& v2, const Vertex& v3)
{
    // get bounding box
    int xMin = std::min({ v1[0], v2[0], v3[0] });
    int xMax = std::max({ v1[0], v2[0], v3[0] });
    int yMin = std::min({ v1[1], v2[1], v3[1] });
    int yMax = std::max({ v1[1], v2[1], v3[1] });

    // equation
    auto lineEquation = [&](const Vertex& v1, const Vertex& v2, int x, int y) -> float
    {
        int a = v2[1] - v1[1];
        int b = v1[0] - v2[0];
        int c = v2[0] * v1[1] - v2[1] * v1[0];
        return static_cast<float>(a * x + b * y + c);
    };

    int a1 = v2[1] - v1[1];
    int a2 = v3[1] - v2[1];
    int a3 = v1[1] - v3[1];
    int b1 = v1[0] - v2[0];
    int b2 = v2[0] - v3[0];
    int b3 = v3[0] - v1[0];
    float e1 = lineEquation(v1, v2, xMin, yMin);
    float e2 = lineEquation(v2, v3, xMin, yMin);
    float e3 = lineEquation(v3, v1, xMin, yMin);
    int xDim = xMax - xMin + 1;

    // draw pixel
    for (int y = yMin; y <= yMax; ++y) {
        for (int x = xMin; x <= xMax; ++x) {
            if (e1 < 0 && e2 < 0 && e3 < 0) {
                // push to list for later rendering
                renderPixel.push_back({ x, y });
            }
            e1 += a1;
            e2 += a2;
            e3 += a3;
        }
        e1 += -xDim * a1 + b1;
        e2 += -xDim * a2 + b2;
        e3 += -xDim * a3 + b3;
    }

}

// animation
void myTimer(int index)
{
    // check if all pixel rendered
    if (index == renderPixel.size())
    {
        // reset after render all pixel
        renderPixel = {};
        isRendering = false;
        return;
    }

    // render current pixel
    const Vertex &pixel = renderPixel.at(index);
    faceGrid.SetPixel(pixel.at(0), pixel.at(1), true);
    glutPostRedisplay();

    // render next pixel
    glutTimerFunc(TIME_INTERVAL, myTimer, ++index);
}

// message printer
void printVertexPixelCoordinate(const Vertex& vertex, const std::string& name)
{
    std::cout << "[info] Vertex \033[91m" << name << "\033[0m at \033[91m(" << vertex[0] << ", " << vertex[1] << ")\033[0m" << std::endl;
}

void printLinePixelCoordinate(const Vertex& vertex, const bool isE)
{
    if (isE)
        std::cout << "\033[93m[debug]\033[0m Line pixel \033[92mE\033[0m at \033[92m(" << vertex[0] << ", " << vertex[1] << ")\033[0m" << std::endl;
    else
        std::cout << "\033[93m[debug]\033[0m Line pixel \033[94mNE\033[0m at \033[94m(" << vertex[0] << ", " << vertex[1] << ")\033[0m" << std::endl;
}

void printLineRegion(const Vertex& v1, const Vertex& v2, const std::string& name)
{
    std::cout << "\033[93m[debug]\033[0m Line \033[96m" << name << "\033[0m is \033[96mregion " << getRegion(v1, v2) << "\033[0m" << std::endl;
}

// for popup menu
void setGridDimension(const int dim)
{
    vertexGrid.SetDimension(dim);
    lineGreenGrid.SetDimension(dim);
    lineBlueGrid.SetDimension(dim);
    faceGrid.SetDimension(dim);
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
