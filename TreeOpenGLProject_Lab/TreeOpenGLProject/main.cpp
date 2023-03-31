﻿#include <iostream>
#include <iomanip>
#include <vector>
#include "GL\freeglut.h" // freeglut
#include "myPopupMenu.h"
#define M_PI 3.1415926535897932384626433832795f

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
// [c]      : input arbitrary axis and theta in console
// 
// translate:
// [↑] : move up
// [↓] : move down
// [←] : move left
// [→] : move right
// 
// ~~~key map~~~

using std::vector;

// function prototypes
void ChangeSize(int, int);
void RenderScene(void);
void SetupRC(void);
void myKeyboard(unsigned char, int, int);
void mySpecialKey(int, int, int);
void myInputArbitraryAxis(void);
void myResetMatrix(void);
void myRotateMatrix(GLfloat, GLfloat, GLfloat, GLfloat);
void myTranslateMatrix(GLfloat, GLfloat, GLfloat);
void myArbitraryRotate(GLfloat, vector<GLfloat>, vector<GLfloat>);
void myMatrixInfo(void);

// These are variable that you will need
// to move your cube
GLfloat tx = 0.0;
GLfloat ty = 0.0;
GLfloat tz = 0.0;
GLfloat thetaX = 0.0;
GLfloat thetaY = 0.0;
GLfloat thetaZ = 0.0;

vector<GLfloat> V1 = { 3, -5, 0 };
vector<GLfloat> V2 = { 3, 5, 0 };
GLfloat arbitraryTheta = 0.0f;

// change rate of Translate and Rotate
const GLfloat deltaT = 0.3f;
const GLfloat deltaR = 4.5f;
const GLfloat axisLength = 7.0f;
constexpr GLfloat deg2rad = M_PI / 180.0f;

vector<GLfloat> commonMatrix = {
    1,0,0,0,
    0,1,0,0,
    0,0,1,0,
    0,0,0,1
};

myPopupMenu* myMenu;

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(400, 400);
    glutInitWindowPosition(600, 80);
    glutCreateWindow("kyubu!?");
    SetupRC();
    glutReshapeFunc(ChangeSize);
    glutDisplayFunc(RenderScene);
    glutKeyboardFunc(myKeyboard);
    glutSpecialFunc(mySpecialKey);

    (new myPopupMenu())->SetupPopupMenu();

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

    // draw x-axis, y-axis, z-axis
    glBegin(GL_LINES);
    // red x-axis
    glColor3f(1, 0, 0);
    glVertex3f(-axisLength, 0, 0);
    glVertex3f(axisLength, 0, 0);
    // green y-axis
    glColor3f(0, 1, 0);
    glVertex3f(0, -axisLength, 0);
    glVertex3f(0, axisLength, 0);
    // blue z-axis
    glColor3f(0, 0, 1);
    glVertex3f(0, 0, -axisLength);
    glVertex3f(0, 0, axisLength);
    glEnd();

    // perform transformation for the cube
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // basic transformation
    myMatrixInfo();
    myRotateMatrix(thetaX, 1, 0, 0);
    myRotateMatrix(thetaY, 0, 1, 0);
    myRotateMatrix(thetaZ, 0, 0, 1);
    myTranslateMatrix(tx, ty, tz);
    // special transformation
    myArbitraryRotate(arbitraryTheta, V1, V2);

    // cube
    glColor3f(0.9f, 0.21f, 0.45f);
    glutSolidCube(6);
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
    case 'f':
        // exit program
        exit(0);
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

void myInputArbitraryAxis(void) {
    // move cursor to (1, 1) and clear the console
    std::cout << "\033[1;1H\033[2J";

    std::cout << "[info]Input Point 1's (x, y, z) ~" << std::endl;
    std::cin >> V1[0] >> V1[1] >> V1[2];
    std::cout << "[info]Input Point 2's (x, y, z) ~" << std::endl;
    std::cin >> V2[0] >> V2[1] >> V2[2];
}

void myResetMatrix(void) {
    for (int i = 0; i < 4; i++) {
        commonMatrix[4 * i] = i == 0 ? 1.0f : 0.0f;
        commonMatrix[4 * i + 1] = i == 1 ? 1.0f : 0.0f;
        commonMatrix[4 * i + 2] = i == 2 ? 1.0f : 0.0f;
        commonMatrix[4 * i + 3] = i == 3 ? 1.0f : 0.0f;
    }
}

void myRotateMatrix(GLfloat angle, GLfloat uX, GLfloat uY, GLfloat uZ) {
    GLfloat RaCos = cos(angle * deg2rad);
    GLfloat RaSin = sin(angle * deg2rad);

    myResetMatrix();

    commonMatrix[0] = RaCos + (1 - RaCos) * uX * uX;
    commonMatrix[1] = (1 - RaCos) * uX * uY + RaSin * uZ;
    commonMatrix[2] = (1 - RaCos) * uX * uZ - RaSin * uY;
    commonMatrix[4] = (1 - RaCos) * uX * uY - RaSin * uZ;
    commonMatrix[5] = RaCos + (1 - RaCos) * uY * uY;
    commonMatrix[6] = (1 - RaCos) * uY * uZ + RaSin * uX;
    commonMatrix[8] = (1 - RaCos) * uX * uZ + RaSin * uY;
    commonMatrix[9] = (1 - RaCos) * uY * uZ - RaSin * uX;
    commonMatrix[10] = RaCos + (1 - RaCos) * uZ * uZ;
   
    glMultMatrixf(commonMatrix.data());
}

void myTranslateMatrix(GLfloat x, GLfloat y, GLfloat z) {
    myResetMatrix();

    commonMatrix[12] = x;
    commonMatrix[13] = y;
    commonMatrix[14] = z;

    glMultMatrixf(commonMatrix.data());
}

void myArbitraryRotate(GLfloat angle, vector<GLfloat> p1, vector<GLfloat> p2) {
    // get unit vector
    GLfloat length = sqrt((p2[0] - p1[0]) * (p2[0] - p1[0]) + (p2[1] - p1[1]) * (p2[1] - p1[1]) + (p2[2] - p1[2]) * (p2[2] - p1[2]));
    GLfloat x = (p2[0] - p1[0]) / length;
    GLfloat y = (p2[1] - p1[1]) / length;
    GLfloat z = (p2[2] - p1[2]) / length;

    myTranslateMatrix(p1[0], p1[1], p1[2]);
    myRotateMatrix(angle, x, y, z);
    myTranslateMatrix(-p1[0], -p1[1], -p1[2]);

    // draw arbitrary axis
    glBegin(GL_LINES);
    glColor3f(1, 1, 0);
    glVertex3f(p1[0], p1[1], p1[2]);
    glVertex3f(p2[0], p2[1], p2[2]);
    glEnd();
}

void myMatrixInfo() {
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
