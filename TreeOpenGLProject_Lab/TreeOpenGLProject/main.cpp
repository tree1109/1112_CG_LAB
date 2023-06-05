#include <iostream>
#include <iomanip>
#include <random>
#include "GL\freeglut.h" // freeglut
#include "main.h"
#include "myPopupMenu.h"
#include "myGrid.h"

// ~~~key map~~~
// [r] : clean up grid and vertex
// ~~~key map~~~

MyGrid colorGrid;

// only for non Crowbar Mode
Vertex v1;
Vertex v2;
Vertex v3;

// only for Crowbar Mode
// vertices list
std::vector<Vertex> vList = {};

// only for non Crowbar Mode
// Normal mode: only show vertex
// Debug mode: show vertex and line
bool onDebugMode = false;

// is crowbar mode???
bool onCrowbarMode = false;

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

    colorGrid.RenderGrid();

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
        if (isRendering)
        {
            std::cout << "\033[91m[error]\033[0m Pixels are rendering!!" << std::endl;
            return;
        }
        // clean up grid and vertex
        colorGrid.RemoveAllPixel();
        currentVertex = CURRENT_VERTEX::V1;
        vList = {};
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
    const GLfloat translateToGrid = static_cast<GLfloat>(colorGrid.GetGridDimension()) + halfOfCellSize;
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
    static bool debug_mode_flag;
    if (isRendering)
    {
        std::cout << "\033[91m[error]\033[0m Pixels are rendering!!" << std::endl;
        return;
    }

    if (onCrowbarMode)
    {
        if (vList.empty())
            colorGrid.RemoveAllPixel();

        // random color
        const float r = getRandomNumber();
        const float g = getRandomNumber();
        const float b = getRandomNumber();

        const Vertex v = { x, y , r, g, b };
        pushToVerticesList(v);
        vertexPainter(v, "v");
    }
    else
    {
        switch (currentVertex)
        {
        case CURRENT_VERTEX::V1:
            colorGrid.RemoveAllPixel();
            v1 = { x, y };
            vertexPainter(v1, "v1");
            currentVertex = CURRENT_VERTEX::V2;
            debug_mode_flag = onDebugMode;
            break;
        case CURRENT_VERTEX::V2:
            v2 = { x, y };
            vertexPainter(v2, "v2");
            if (debug_mode_flag) {
                linePainter(v1, v2, "v1v2");
            }
            currentVertex = CURRENT_VERTEX::V3;
            break;
        case CURRENT_VERTEX::V3:
            v3 = { x, y };
            vertexPainter(v3, "v3");
            if (debug_mode_flag)
            {
                linePainter(v2, v3, "v2v3");
                linePainter(v3, v1, "v3v1");
                facePainter(v1, v2, v3, "v1v2v3");
            }
            currentVertex = CURRENT_VERTEX::V1;
            break;
        }
    }
}

void vertexPainter(const Vertex& v, const std::string& name)
{
    // for compatibility
    const Vec2i v_old = { static_cast<int>(v.x), static_cast<int>(v.y) };

    if (onCrowbarMode)
    {
        colorGrid.SetPixel(v.x, v.y, { v.r, v.g, v.b });
        printVertexPixelCoordinate(v_old, "v");
    }
    else
    {
        // vertex color: red
        colorGrid.SetVertexPixel(v_old[0], v_old[1]);
        printVertexPixelCoordinate(v_old, name);
    }
}

void linePainter(const Vertex& v1, const Vertex& v2, const std::string& name)
{
    // for compatibility
    const Vec2i v1_old = { static_cast<int>(v1.x), static_cast<int>(v1.y) };
    const Vec2i v2_old = { static_cast<int>(v2.x), static_cast<int>(v2.y) };

    if (onCrowbarMode)
    {
        
    }
    else
    {
        // line color: green(E) or blue(NE)
        midpointAlgorithm(v1_old[0], v1_old[1], v2_old[0], v2_old[1]);
        printLineRegion(v1_old, v2_old, name);
    }

    
}

void facePainter(const Vertex& v1, const Vertex& v2, const Vertex& v3, const std::string& name)
{
    // for compatibility
    const Vec2i v1_old = { static_cast<int>(v1.x), static_cast<int>(v1.y) };
    const Vec2i v2_old = { static_cast<int>(v2.x), static_cast<int>(v2.y) };
    const Vec2i v3_old = { static_cast<int>(v3.x), static_cast<int>(v3.y) };

    halfSpaceTest(v1_old, v2_old, v3_old);
    if (isRendering)
    {
        std::cout << "\033[91m[error]\033[0m Pixels are rendering!!" << std::endl;
        return;
    }
    // start rendering
    isRendering = true;
    glutTimerFunc(TIME_INTERVAL, myTimer, 0);
}

int getRegion(const Vec2i& v1, const Vec2i& v2)
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
            if (isE) colorGrid.SetEPixel(y, x);
            else colorGrid.SetNEPixel(y, x);
            printLinePixelCoordinate({ y, x }, isE);
        }
        else {
            if (isE) colorGrid.SetEPixel(x, y);
            else colorGrid.SetNEPixel(x, y);
            printLinePixelCoordinate({ x, y }, isE);
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

void halfSpaceTest(const Vec2i& v1, const Vec2i& v2, const Vec2i& v3)
{
    // get bounding box
    int xMin = std::min({ v1[0], v2[0], v3[0] });
    int xMax = std::max({ v1[0], v2[0], v3[0] });
    int yMin = std::min({ v1[1], v2[1], v3[1] });
    int yMax = std::max({ v1[1], v2[1], v3[1] });

    // equation
    auto lineEquation = [&](const Vec2i& v1, const Vec2i& v2, int x, int y) -> float
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
                pushToPixelRenderQueue({ x, y , 1.0f, 1.0f, 0.5f });
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

void pushToVerticesList(const Vertex& v)
{
    vList.push_back(v);
}

void pushToPixelRenderQueue(const Vertex& v)
{
    renderPixel.push_back(v);
}

float getRandomNumber()
{
    // random number generator
    // more at https://blog.gtwang.org/programming/cpp-random-number-generator-and-probability-distribution-tutorial/
    static std::random_device randomDevice;
    static std::mt19937 generator(randomDevice());
    static std::normal_distribution<double> normal(0.7, 0.4);

    return static_cast<float>(normal(generator));
}

// crowbar
// only convex polygon
void crow(const std::vector<Vertex>& v_list, int v_num)
{
    int index_min = 0;
    // find bottom vertex index
    for (int i=1; i<v_num; ++i)
        if (v_list[index_min].y > v_list[i].y)
            index_min = i;

    scanY(v_list, v_num, index_min);
}

void scanY(const std::vector<Vertex>& v_list, int v_num, int v_index)
{
    // upper endpoint index
    int left_index;
    int right_index;
    // upper endpoint y
    int left_upper_endpoint_y;
    int right_upper_endpoint_y;
    // current edge and delta
    Vertex left_edge;
    Vertex left_edge_delta;
    Vertex right_edge;
    Vertex right_edge_delta;
    // number of remaining vertices
    int rem_v_num;
    // current scan line
    int y;

    left_index = right_index = v_index;
    left_upper_endpoint_y = right_upper_endpoint_y = y = static_cast<int>(ceil(v_list[v_index].y));
    
    for (rem_v_num = v_num; rem_v_num > 0;)
    {
        // find appropriate left edge
        while (left_upper_endpoint_y <= y && rem_v_num > 0)
        {
            v_index--;
            // go clockwise
            if (v_index < 0)
                v_index = v_num - 1;

            left_upper_endpoint_y = static_cast<int>(ceil(v_list[v_index].y));
            // replace left edge
            if (y < left_upper_endpoint_y)
            {
                differenceY(v_list[left_index], v_list[v_index], left_edge, left_edge_delta, y);
                rem_v_num--; // after found the right upper index, decrease number
            }
            left_index = v_index;
        }
        // find appropriate right edge
        while (right_upper_endpoint_y <= y && rem_v_num > 0)
        {
            v_index++;
            // go counter-clockwise
            if (v_index >= v_num)
                v_index = 0;

            right_upper_endpoint_y = static_cast<int>(ceil(v_list[v_index].y));
            // replace right edge
            if (y < right_upper_endpoint_y)
            {
                differenceY(v_list[right_index], v_list[v_index], right_edge, right_edge_delta, y);
                rem_v_num--; // after found the right upper index, decrease number
            }
            right_index = v_index;
        }
        // while l & r span y (the current scanline)
        // draw the span
        for (; y < left_upper_endpoint_y && y < right_upper_endpoint_y; ++y)
        {
            // scan and interpolate edges
            scanX(left_edge, right_edge, y);
            increment(left_edge, left_edge_delta);
            increment(right_edge, right_edge_delta);
        }
    }
}

void scanX(Vertex& left_edge, Vertex& right_edge, int y)
{
    const int left_x = static_cast<int>(ceil(left_edge.x));
    const int right_x = static_cast<int>(ceil(right_edge.x));
    Vertex s;
    Vertex delta_s;

    if (left_x < right_x)
    {
        differenceX(left_edge, right_edge, s, delta_s, left_x);
        for (int x = left_x; x < right_x; ++x)
        {
            pushToPixelRenderQueue({ x, y ,1.0f ,0.25f ,0.5f});
            increment(s, delta_s);
        }
    }
}

void difference(const Vertex& v1, const Vertex& v2, Vertex& edge, Vertex& delta_edge, float distance, float fix)
{
    delta_edge.x = (v2.x - v1.x) / distance;
    edge.x = v1.x + fix * delta_edge.x;
}

void differenceY(const Vertex& v1, const Vertex& v2, Vertex& edge, Vertex& delta_edge, int y)
{
    const float distance_y = v2.y - v1.y;
    const float fix_y = y - v1.y;
    difference(v1, v2, edge, delta_edge, distance_y, fix_y);
}

void differenceX(const Vertex& v1, const Vertex& v2, Vertex& edge, Vertex& delta_edge, int x)
{
    const float distance_x = v2.x - v1.x;
    const float fix_x = x - v1.x;
    difference(v1, v2, edge, delta_edge, distance_x, fix_x);
}

void increment(Vertex& edge, const Vertex& delta)
{
    edge.x += delta.x;
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
    if (!colorGrid.isPixelColorFilledAt(pixel.x, pixel.y))
        colorGrid.SetPixel(pixel.x, pixel.y, { pixel.r, pixel.g, pixel.b });
    glutPostRedisplay();

    // render next pixel
    glutTimerFunc(TIME_INTERVAL, myTimer, ++index);
}

// message printer
void printVertexPixelCoordinate(const Vec2i& vertex, const std::string& name)
{
    std::cout << "[info] Vertex \033[91m" << name << "\033[0m at \033[91m(" << vertex[0] << ", " << vertex[1] << ")\033[0m" << std::endl;
}

void printLinePixelCoordinate(const Vec2i& vertex, const bool isE)
{
    if (isE)
        std::cout << "\033[93m[debug]\033[0m Line pixel \033[92mE\033[0m at \033[92m(" << vertex[0] << ", " << vertex[1] << ")\033[0m" << std::endl;
    else
        std::cout << "\033[93m[debug]\033[0m Line pixel \033[94mNE\033[0m at \033[94m(" << vertex[0] << ", " << vertex[1] << ")\033[0m" << std::endl;
}

void printLineRegion(const Vec2i& v1, const Vec2i& v2, const std::string& name)
{
    std::cout << "\033[93m[debug]\033[0m Line \033[96m" << name << "\033[0m is \033[96mregion " << getRegion(v1, v2) << "\033[0m" << std::endl;
}

// for popup menu
void setGridDimension(const int dim)
{
    if (isRendering)
    {
        std::cout << "\033[91m[error]\033[0m Pixels are rendering!!" << std::endl;
        return;
    }
    colorGrid.SetDimension(dim);
    currentVertex = CURRENT_VERTEX::V1;
    glutPostRedisplay();
}

void switchDebugMode()
{
    if (onCrowbarMode)
    {
        std::cout << "\033[91m[error]\033[0m Only for Non Crowbar Mode !!" << std::endl;
        return;
    }

    onDebugMode = !onDebugMode;
    if (onDebugMode)
        std::cout << "[info] Switch to \033[93mDebug Mode\033[0m" << std::endl;
    else
        std::cout << "[info] Switch to \033[92mNormal Mode\033[0m" << std::endl;
    glutPostRedisplay();
}

void drawEdges()
{
    if (!onCrowbarMode)
    {
        std::cout << "\033[91m[error]\033[0m Only for Crowbar Mode!!" << std::endl;
        return;
    }

    if (isRendering)
    {
        std::cout << "\033[91m[error]\033[0m Pixels are rendering!!" << std::endl;
        return;
    }

    std::cout << "[info] Start draw polygon." << std::endl;

    // TODO 連線產生線段
    // TODO 線段根據端點顏色插植
}

void drawPolygon()
{
    if (!onCrowbarMode)
    {
        std::cout << "\033[91m[error]\033[0m Only for Crowbar Mode!!" << std::endl;
        return;
    }

    if (isRendering)
    {
        std::cout << "\033[91m[error]\033[0m Pixels are rendering!!" << std::endl;
        return;
    }

    if (vList.empty())
    {
        std::cout << "\033[91m[error]\033[0m Too few vertices!! At least 1." << std::endl;
        return;
    }

    std::cout << "[info] Start draw polygon." << std::endl;

    crow(vList, vList.size());
    vList = {};

    // start rendering
    isRendering = true;
    glutTimerFunc(TIME_INTERVAL, myTimer, 0);
}

void switchCrowbarMode()
{
    if (isRendering)
    {
        std::cout << "\033[91m[error]\033[0m Pixels are rendering!!" << std::endl;
        return;
    }

    onCrowbarMode = !onCrowbarMode;
    if (onCrowbarMode)
    {
        std::cout << "[info] \033[96mCrowbar Mode\033[0m switch \033[92mon\033[0m!" << std::endl;
        colorGrid.RemoveAllPixel();
    }
    else
    {
        std::cout << "[info] \033[96mCrowbar Mode\033[0m switch \033[91moff\033[0m!" << std::endl;
        colorGrid.RemoveAllPixel();
        currentVertex = CURRENT_VERTEX::V1;
        vList = {};
    }
    glutPostRedisplay();
}
