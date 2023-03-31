
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
/*** freeglut***/
#include ".\GL\freeglut.h"

void ChangeSize(int, int);
void RenderScene(void);
void SetupRC();
void myKeyboard(unsigned char, int, int);
void mySpecialKey(int, int, int);

GLfloat tx;
GLfloat ty;
GLfloat tz;
GLfloat thetaX;
GLfloat thetaY;
GLfloat thetaZ;

int main(int argc, char** argv)
{
    // These are variable that you will need
    // to move your cube
    tx = 0;
    ty = 0;
    tz = 0;
    thetaX = 0;
    thetaY = 0;
    thetaZ = 0;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(400, 400);
    glutInitWindowPosition(600, 80);
    glutCreateWindow("Moving Cube");
    SetupRC();
    glutReshapeFunc(ChangeSize);
    glutDisplayFunc(RenderScene);
    glutKeyboardFunc(myKeyboard);
    glutSpecialFunc(mySpecialKey);

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
    static const GLfloat axisLength = 5;

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

    // perform transformation for the cube
    glRotatef(thetaX, 1, 0, 0);
    glRotatef(thetaY, 0, 1, 0);
    glRotatef(thetaZ, 0, 0, 1);
    glTranslatef(tx, ty, tz);

    // cube
    glColor3f(0.9f, 0.12f, 0.45f);
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
    static const GLfloat deltaR = 5;
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
    case 'z':
        // change the rotation angle thetaZ along z-axis
        thetaZ -= deltaR;
        break;
    case 'x':
        // change the rotation angle thetaZ along z-axis
        thetaZ += deltaR;
        break;
    default:
        break;
    }
    glutPostRedisplay();
}
void mySpecialKey(int key, int x, int y)
{
    static const GLfloat deltaT = 0.5;
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