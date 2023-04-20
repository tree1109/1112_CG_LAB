#include <iostream>
#include <iomanip>
#include "GL\freeglut.h" // freeglut
#include "main.h"
#include "myPopupMenu.h"
#include "myMatrix.h"
#include "myObject.h"

#define SHOW_DEBUG_INFO false

// ~~~key map~~~
// [r] : reset to origin
// 
// rotate:
// [w], [s] : rotate by x-axis
// [a], [d] : rotate by y-axis
// [q], [e] : rotate by z-axis
// 
// arbitrary rotate:
// [z], [x] : rotate by arbitrary axis
// [c]      : input v1 and v2 coordinates with "x y z x y z" format then press [Enter] to input
// [Mouse Left] : press down to set v1, press up to set v2
// 
// translate:
// [↑] : move up
// [↓] : move down
// [←] : move left
// [→] : move right
// 
// scale:
// [Scroll up]   : scale up
// [Scroll down] : scale down
// 
// ~~~key map~~~

// function prototypes
void ChangeSize(int, int);
void RenderScene(void);
void SetupRC(void);
void myKeyboard(unsigned char, int, int);
void mySpecialKey(int, int, int);
void myMouse(int, int, int, int);
void myInputArbitraryAxis(void);
void myDrawAxis(GLfloat);
void myDebugInfo(void);
void printMouseWindowCoordinate(int, int, bool);

// demo object file path
std::string modelsDirPath = "C:\\Users\\zhnzh\\Desktop\\1112_CG_LAB\\TreeOpenGLProject_Lab\\TreeOpenGLProject\\models\\";
std::string teapotObjPath = modelsDirPath + "teapot.obj";
std::string teddyObjPath = modelsDirPath + "teddy.obj";
std::string octahedronObjPath = modelsDirPath + "octahedron.obj";
std::string gourdObjPath = modelsDirPath + "gourd.obj";
// demo object
myObject teapot;
myObject teddy;
myObject octahedron;
myObject gourd;

// show obj, render mode, color mode status
OBJECT currentObject = OBJECT::TEAPOT;
RENDER_MODE currentRenderMode = RENDER_MODE::FACES;
COLOR_MODE currentColorMode = COLOR_MODE::SINGLE;

// These are variable that you will need
// to move your cube
// basic
GLfloat tx = 0.0;
GLfloat ty = 0.0;
GLfloat tz = 0.0;
GLfloat thetaX = 0.0;
GLfloat thetaY = 0.0;
GLfloat thetaZ = 0.0;
GLfloat scale = 1.0f;
// arbitrary
GLfloat V1[] = { -5, -5, -5 };
GLfloat V2[] = { 5, 5, 5 };
GLfloat arbitraryTheta = 0.0f;

// change rate of Translate and Rotate
const GLfloat deltaT = 0.3f;
const GLfloat deltaR = 4.5f;
const GLfloat deltaS = 1.1f;
const GLfloat axisLength = 7.0f;

// scaling coefficient
GLfloat justifyScale = 1.0f;

myMatrix TransformMatrix;

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
    glutSpecialFunc(mySpecialKey);
    glutMouseFunc(myMouse);

    myPopupMenu::CreatePopupMenu();

    // preload demo object file
    teapot.loadObjectFile(teapotObjPath);
    teddy.loadObjectFile(teddyObjPath);
    octahedron.loadObjectFile(octahedronObjPath);
    gourd.loadObjectFile(gourdObjPath);

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

void RenderScene(void)
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW); // load the model view matrix
    glLoadIdentity();
    gluLookAt(0, 0, 10.0f, 0, 0, 0, 0, 1, 0);

    // perform transformation for the cube
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // draw x-axis, y-axis, z-axis
    myDrawAxis(axisLength);

    if (SHOW_DEBUG_INFO)
        myDebugInfo();

    // ~~~object~~~
    // Transform
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity(); // reset model matrix
    teapot.setTransformation({tx, ty, tz}, {thetaX, thetaY, thetaZ}, scale);
    teapot.fitToWindow(); // justify object scale
    teapot.setArbitraryRotate(arbitraryTheta, { V1[0], V1[1], V1[2] }, { V2[0], V2[1], V2[2] });
    teapot.doTransformation();
    // Rendering
    switch (currentObject)
    {
    case OBJECT::TEAPOT:
        teapot.setRenderMode(currentRenderMode);
        teapot.setColorMode(currentColorMode);
        teapot.drawObject();
        break;
    case OBJECT::TEDDY:
        teddy.setRenderMode(currentRenderMode);
        teddy.setColorMode(currentColorMode);
        teddy.drawObject();
        break;
    case OBJECT::OCTAHEDRON:
        octahedron.setRenderMode(currentRenderMode);
        octahedron.setColorMode(currentColorMode);
        octahedron.drawObject();
        break;
    case OBJECT::GOURD:
        gourd.setRenderMode(currentRenderMode);
        gourd.setColorMode(currentColorMode);
        gourd.drawObject();
        break;
    default:
        break;
    }
    // ~~~object~~~

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
        // reset translation & rotation
        tx = 0;
        ty = 0;
        tz = 0;
        thetaX = 0;
        thetaY = 0;
        thetaZ = 0;
        scale = 1;
        arbitraryTheta = 0;
        break;
    case 'a':
        // change the rotation angle thetaY along y-axis
        thetaY -= deltaR;
        break;
    case 'd':
        // change the rotation angle thetaY along y-axis
        thetaY += deltaR;
        break;
    case 'w':
        // change the rotation angle thetaX along x-axis
        thetaX -= deltaR;
        break;
    case 's':
        // change the rotation angle thetaX along x-axis
        thetaX += deltaR;
        break;
    case 'e':
        // change the rotation angle thetaZ along z-axis
        thetaZ -= deltaR;
        break;
    case 'q':
        // change the rotation angle thetaZ along z-axis
        thetaZ += deltaR;
        break;
    case 'c':
        // input arbitrary axis and theta in console
        myInputArbitraryAxis();
        break;
    case 'z':
        arbitraryTheta += deltaR;
        break;
    case 'x':
        arbitraryTheta -= deltaR;
        break;
    default:
        break;
    }
    glutPostRedisplay();
}

void mySpecialKey(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_LEFT:
        // change the translation along x-axis
        tx -= deltaT;
        break;
    case GLUT_KEY_RIGHT:
        // change the translation along x-axis
        tx += deltaT;
        break;
    case GLUT_KEY_UP:
        // change the translation along y-axis
        ty += deltaT;
        break;
    case GLUT_KEY_DOWN:
        // change the translation along y-axis
        ty -= deltaT;
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
    GLfloat worldX = (GLfloat)x / glutGet(GLUT_WINDOW_WIDTH) * 20 - 10;
    GLfloat worldY = (1 - (GLfloat)y / glutGet(GLUT_WINDOW_HEIGHT)) * 20 - 10;
    const GLfloat depth = 0.0f;

    switch (button)
    {
    case GLUT_LEFT_BUTTON:
        if (state == GLUT_DOWN) {
            V1[0] = worldX;
            V1[1] = worldY;
            V1[2] = depth;
            printMouseWindowCoordinate(x, y, true);
        } else if (state == GLUT_UP) {
            V2[0] = worldX;
            V2[1] = worldY;
            V2[2] = depth;
            printMouseWindowCoordinate(x, y, false);
        }
        break;
    case 3: // scroll up
        // change the scale
        if (state == GLUT_DOWN)
            scale *= deltaS;
        break;
    case 4: // scroll down
        // change the scale
        if (state == GLUT_DOWN)
            scale /= deltaS;
        break;
    default:
        break;
    }
    glutPostRedisplay();
}

void myInputArbitraryAxis(void) {
    // move cursor to (1, 1) and clear the console
    std::cout << "\033[1;1H\033[2J";

    std::cout << "[info]Input v1 and v2 coordinate with \"x y z x y z\" format~" << std::endl;
    std::cin >> V1[0] >> V1[1] >> V1[2] >> V2[0] >> V2[1] >> V2[2];
}

void myDrawAxis(GLfloat length) {
    // draw x-axis, y-axis, z-axis
    glBegin(GL_LINES);
    // red x-axis
    glColor3f(1, 0, 0);
    glVertex3f(-length, 0, 0);
    glVertex3f(length, 0, 0);
    // green y-axis
    glColor3f(0, 1, 0);
    glVertex3f(0, -length, 0);
    glVertex3f(0, length, 0);
    // blue z-axis
    glColor3f(0, 0, 1);
    glVertex3f(0, 0, -length);
    glVertex3f(0, 0, length);
    glEnd();
}

void myDebugInfo() {
    // move cursor to (1, 1)
    std::cout << "\033[1;1H\033[0J";

    // set precision
    std::cout << std::fixed << std::setprecision(1);

    // print translation
    std::cout << "[debug] (tx, ty, tz)             : ("
        << std::setw(6) << tx << ", "
        << std::setw(6) << ty << ", "
        << std::setw(6) << tz << ")" << std::endl;

    // print rotation angle
    std::cout << "[debug] (thetaX, thetaY, thetaZ) : ("
        << std::setw(6) << thetaX << ", "
        << std::setw(6) << thetaY << ", "
        << std::setw(6) << thetaZ << ")" << std::endl;

    // print arbitrary axis
    std::cout << "[debug] (V1, V2)                 : ("
        << std::setw(6) << V1[0] << ", "
        << std::setw(6) << V1[1] << ", "
        << std::setw(6) << V1[2] << "), ("
        << std::setw(6) << V2[0] << ", "
        << std::setw(6) << V2[1] << ", "
        << std::setw(6) << V2[2] << ")" << std::endl;

    // print arbitrary rotation angle
    std::cout << "[debug] (arbitraryTheta)         : ("
        << std::setw(6) << arbitraryTheta << ")" << std::endl;
}

void printMouseWindowCoordinate(int x, int y, bool isDown) {
    if (isDown) {
        std::cout << "[info] : mouse \033[93mdown\033[0m at (" << x << ", " << y << ")" << std::endl;
    } else {
        std::cout << "[info] : mouse \033[92mup\033[0m at (" << x << ", " << y << ")" << std::endl;
    }
}
