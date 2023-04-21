#include <iostream>
#include <iomanip>
#include "GL\freeglut.h" // freeglut
#include "main.h"
#include "myPopupMenu.h"
#include "myMatrix.h"
#include "myObject.h"
#include "vec3.h"

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
myObject& currentObject = teapot;

// render mode, color mode status
RENDER_MODE currentRenderMode = RENDER_MODE::FACES;
COLOR_MODE currentColorMode = COLOR_MODE::SINGLE;

// These are variable that you will need
// to move your cube
// basic
vec3 translate = { 0.0f, 0.0f, 0.0f };
vec3 rotate = { 0.0f, 0.0f, 0.0f };
GLfloat scale = 1.0f;
// arbitrary
vec3 v1 = { -5, -5, -5 };
vec3 v2 = { 5, 5, 5 };
GLfloat arbitraryTheta = 0.0f;

// change rate of Translate and Rotate
const GLfloat deltaT = 0.3f;
const GLfloat deltaR = 4.5f;
const GLfloat deltaS = 1.1f;
const GLfloat axisLength = 7.0f;

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
    currentObject.setTransformation(translate, rotate, scale);
    currentObject.fitToWindow(); // justify object scale
    currentObject.setArbitraryRotate(arbitraryTheta, v1, v2);
    currentObject.doTransformation();
    // Rendering
    currentObject.setRenderMode(currentRenderMode);
    currentObject.setColorMode(currentColorMode);
    currentObject.drawObject();
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
        translate = { 0, 0, 0 };
        rotate = { 0, 0, 0 };
        scale = 1;
        arbitraryTheta = 0;
        break;
    case 'a':
        // change the rotation angle thetaY along y-axis
        rotate.y -= deltaR;
        break;
    case 'd':
        // change the rotation angle thetaY along y-axis
        rotate.y += deltaR;
        break;
    case 'w':
        // change the rotation angle thetaX along x-axis
        rotate.x -= deltaR;
        break;
    case 's':
        // change the rotation angle thetaX along x-axis
        rotate.x += deltaR;
        break;
    case 'e':
        // change the rotation angle thetaZ along z-axis
        rotate.z -= deltaR;
        break;
    case 'q':
        // change the rotation angle thetaZ along z-axis
        rotate.z += deltaR;
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
        translate.x -= deltaT;
        break;
    case GLUT_KEY_RIGHT:
        // change the translation along x-axis
        translate.x += deltaT;
        break;
    case GLUT_KEY_UP:
        // change the translation along y-axis
        translate.y += deltaT;
        break;
    case GLUT_KEY_DOWN:
        // change the translation along y-axis
        translate.y -= deltaT;
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
            v1.x = worldX;
            v1.y = worldY;
            v1.z = depth;
            printMouseWindowCoordinate(x, y, true);
        } else if (state == GLUT_UP) {
            v2.x = worldX;
            v2.y = worldY;
            v2.z = depth;
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

    std::cout << "[info]Input v1 and v2 coordinate with format" << std::endl;
    std::cout << "(x y z x y z) : ";
    std::cin >> v1.x >> v1.y >> v1.z >> v2.x >> v2.y >> v2.z;
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
    std::cout << "[debug] translate (x, y, z) : ("
        << std::setw(6) << translate.x << ", "
        << std::setw(6) << translate.y << ", "
        << std::setw(6) << translate.z << ")" << std::endl;

    // print rotation angle
    std::cout << "[debug] rotate (x, y, z)    : ("
        << std::setw(6) << rotate.x << ", "
        << std::setw(6) << rotate.y << ", "
        << std::setw(6) << rotate.z << ")" << std::endl;

    // print arbitrary axis
    std::cout << "[debug] v1, v2 (x, y, z)    : ("
        << std::setw(6) << v1.x << ", "
        << std::setw(6) << v1.y << ", "
        << std::setw(6) << v1.z << "), ("
        << std::setw(6) << v2.x << ", "
        << std::setw(6) << v2.y << ", "
        << std::setw(6) << v2.z << ")" << std::endl;

    // print arbitrary rotation angle
    std::cout << "[debug]  arbitraryTheta     : ("
        << std::setw(6) << arbitraryTheta << ")" << std::endl;
}

void printMouseWindowCoordinate(int x, int y, bool isDown) {
    if (isDown) {
        std::cout << "[info] : mouse \033[93mdown\033[0m at (" << x << ", " << y << ")" << std::endl;
    } else {
        std::cout << "[info] : mouse \033[92mup\033[0m at (" << x << ", " << y << ")" << std::endl;
    }
}

void setCurrentObject(OBJECT seletedObj)
{
    switch (seletedObj)
    {
    case OBJECT::TEAPOT:
        currentObject = teapot;
        break;
    case OBJECT::TEDDY:
        currentObject = teddy;
        break;
    case OBJECT::OCTAHEDRON:
        currentObject = octahedron;
        break;
    case OBJECT::GOURD:
        currentObject = gourd;
        break;
    default:
        std::cout << "[error] : unknown object" << std::endl;
        currentObject = teapot;
        break;
    }
}
