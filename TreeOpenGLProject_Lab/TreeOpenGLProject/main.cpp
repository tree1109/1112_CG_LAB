#include <iostream>
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
void myUpdateMatrix(void);
void myInputArbitraryAxis(void);
void myArbitraryRotate(void);
void myDebugInfo(void);

// These are variable that you will need
// to move your cube
GLfloat tx = 0.0;
GLfloat ty = 0.0;
GLfloat tz = 0.0;
GLfloat thetaX = 0.0;
GLfloat thetaY = 0.0;
GLfloat thetaZ = 0.0;

vector<GLfloat> V1 = { 0, 0, 0 };
vector<GLfloat> V2 = { 0, 0, 0 };
GLfloat arbitraryTheta = 0.0f;

// change rate of Translate and Rotate
const GLfloat deltaT = 0.3f;
const GLfloat deltaR = 4.5f;

const GLfloat axisLength = 7.0f;

vector<GLfloat> translateMatrix = {
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
};

vector<GLfloat> rotateXMatrix = {
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
};

vector<GLfloat> rotateYMatrix = {
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
};

vector<GLfloat> rotateZMatrix = {
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
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
    glMatrixMode(GL_MODELVIEW); // load the modelview matrix
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

    myDebugInfo();
    // perform transformation for the cube
    myUpdateMatrix();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // lab4
    //myArbitraryRotate();

    glMultMatrixf(rotateXMatrix.data());
    glMultMatrixf(rotateYMatrix.data());
    glMultMatrixf(rotateZMatrix.data());
    glMultMatrixf(translateMatrix.data());

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
    //case 'z':
    //    theta += deltaR;
    //    break;
    //case 'x':
    //    theta -= deltaR;
    //    break;
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

void myUpdateMatrix() {
    const GLfloat rad = M_PI / 180.0f;
    GLfloat rotateXSin = sin(thetaX * rad);
    GLfloat rotateXCos = cos(thetaX * rad);
    GLfloat rotateYSin = sin(thetaY * rad);
    GLfloat rotateYCos = cos(thetaY * rad);
    GLfloat rotateZSin = sin(thetaZ * rad);
    GLfloat rotateZCos = cos(thetaZ * rad);

    // update translate matrix
    translateMatrix[12] = tx;
    translateMatrix[13] = ty;
    translateMatrix[14] = tz;

    // update rotate x-axis matrix
    rotateXMatrix[5] = rotateXCos;
    rotateXMatrix[6] = rotateXSin;
    rotateXMatrix[9] = -rotateXSin;
    rotateXMatrix[10] = rotateXCos;

    // update rotate y-axis matrix
    rotateYMatrix[0] = rotateYCos;
    rotateYMatrix[2] = -rotateYSin;
    rotateYMatrix[8] = rotateYSin;
    rotateYMatrix[10] = rotateYCos;

    // update rotate z-axis matrix
    rotateZMatrix[0] = rotateZCos;
    rotateZMatrix[1] = rotateZSin;
    rotateZMatrix[4] = -rotateZSin;
    rotateZMatrix[5] = rotateZCos;
}

void myInputArbitraryAxis(void) {
    // move cursor to (1, 1) and clear the console
    std::cout << "\033[1;1H\033[2J";

    std::cout << "[info]Input Point 1's (x, y, z) ~" << std::endl;
    std::cin >> V1[0] >> V1[1] >> V1[2];
    std::cout << "[info]Point 1 is (" << std::setw(8) << V1[0] << ", " << std::setw(8) << V1[1] << ", " << std::setw(8) << V1[2] << ")" << std::endl;
    std::cout << "[info]Input Point 2's (x, y, z) ~" << std::endl;
    std::cin >> V2[0] >> V2[1] >> V2[2];
    std::cout << "[info]Point 2 is (" << std::setw(8) << V2[0] << ", " << std::setw(8) << V2[1] << ", " << std::setw(8) << V2[2] << ")" << std::endl;
    std::cout << "[info]Input the angle of rotation ~" << std::endl;
    std::cin >> arbitraryTheta;
}

void myArbitraryRotate(vector<GLfloat> V1, vector<GLfloat> V2, GLfloat rotateTheta) {
    const GLfloat rad = M_PI / 180.0f;

    // get unit vector
    GLfloat length = sqrt((V2[0] - V1[0]) * (V2[0] - V1[0]) + (V2[1] - V1[1]) * (V2[1] - V1[1]) + (V2[2] - V1[2]) * (V2[2] - V1[2]));
    vector<GLfloat> unitVector = { (V2[0] - V1[0]) / length, (V2[1] - V1[1]) / length, (V2[2] - V1[2]) / length };

    //GLfloat rotateXSin = b / sqrt(b * b + c * c);
    //GLfloat rotateXCos = c / sqrt(b * b + c * c);
    //GLfloat rotateYSin = a / sqrt(a * a + c * c);
    //GLfloat rotateYCos = c / sqrt(a * a + c * c);
    //GLfloat rotateZSin = sin(theta * rad);
    //GLfloat rotateZCos = cos(theta * rad);

    //GLfloat rotateXMatrix[] = {
    //    1.0f, 0.0f, 0.0f, 0.0f,
    //    0.0f, rotateXCos, rotateXSin, 0.0f,
    //    0.0f, -rotateXSin, rotateXCos, 0.0f,
    //    0.0f, 0.0f, 0.0f, 1.0f
    //};

    //GLfloat rotateYMatrix[] = {
    //    rotateYCos, 0.0f, -rotateYSin, 0.0f,
    //    0.0f, 1.0f, 0.0f, 0.0f,
    //    rotateYSin, 0.0f, rotateYCos, 0.0f,
    //    0.0f, 0.0f, 0.0f, 1.0f
    //};

    //GLfloat rotateZMatrix[] = {
    //    rotateZCos, rotateZSin, 0.0f, 0.0f,
    //    -rotateZSin, rotateZCos, 0.0f, 0.0f,
    //    0.0f, 0.0f, 1.0f, 0.0f,
    //    0.0f, 0.0f, 0.0f, 1.0f
    //};

    //// inverse transform
    //GLfloat inverseTranslateMatrix[] = {
    //    1.0f, 0.0f, 0.0f, 0.0f,
    //    0.0f, 1.0f, 0.0f, 0.0f,
    //    0.0f, 0.0f, 1.0f, 0.0f,
    //    -tx, -ty, -tz, 1.0f
    //};

    //GLfloat inverseRotateXMatrix[] = {
    //    1.0f, 0.0f, 0.0f, 0.0f,
    //    0.0f, rotateXCos, -rotateXSin, 0.0f,
    //    0.0f, rotateXSin, rotateXCos, 0.0f,
    //    0.0f, 0.0f, 0.0f, 1.0f
    //};

    //GLfloat inverseRotateYMatrix[] = {
    //    rotateYCos, 0.0f, rotateYSin, 0.0f,
    //    0.0f, 1.0f, 0.0f, 0.0f,
    //    -rotateYSin, 0.0f, rotateYCos, 0.0f,
    //    0.0f, 0.0f, 0.0f, 1.0f
    //};

    //// draw x-axis, y-axis, z-axis
    //glBegin(GL_LINES);
    //// red x-axis
    //glColor3f(0, 1, 1);
    //glVertex3f((v1_x + v2_x) / 2 + axisLength * a, (v1_y + v2_y) / 2 + axisLength * b, (v1_z + v2_z) / 2 + axisLength * c);
    //glVertex3f((v1_x + v2_x) / 2 - axisLength * a, (v1_y + v2_y) / 2 - axisLength * b, (v1_z + v2_z) / 2 - axisLength * c);
    //glEnd();

    //// translate
    //glMultMatrixf(translateMatrix);
    //// rotate x alpha
    //glMultMatrixf(rotateXMatrix);
    //// rotate y beta
    //glMultMatrixf(rotateYMatrix);
    //// rotate z theta
    //glMultMatrixf(rotateZMatrix);
    //// inverse rotate y beta
    //glMultMatrixf(inverseRotateYMatrix);
    //// inverse rotate x alpha
    //glMultMatrixf(inverseRotateXMatrix);
    //// inverse translate
    //glMultMatrixf(inverseTranslateMatrix);
}

void myDebugInfo() {
    // move cursor to (1, 1)
    std::cout << "\033[1;1H\033[2J";


    // set precision
    std::cout << std::fixed << std::setprecision(1);

    // print translation
    std::cout << "[debug] (tx, ty, tz)             : ("
        << std::setw(8) << tx << ", "
        << std::setw(8) << ty << ", "
        << std::setw(8) << tz << ")" << std::endl;

    // print rotation angle
    std::cout << "[debug] (thetaX, thetaY, thetaZ) : ("
        << std::setw(8) << thetaX << ", "
        << std::setw(8) << thetaY << ", "
        << std::setw(8) << thetaZ << ")" << std::endl;
}
