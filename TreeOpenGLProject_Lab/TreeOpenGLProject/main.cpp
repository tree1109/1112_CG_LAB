
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
/*** freeglut***/
#include ".\GL\freeglut.h"

#define MENU_FLAT 0
#define MENU_SMOOTH 1

void ChangeSize(int , int );
void RenderScene(void);
int buildPopupMenu(void);
void mainMenu(int);

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(400,400);
    glutInitWindowPosition(600,80);
    glutCreateWindow("uwu");

    buildPopupMenu();
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutReshapeFunc(ChangeSize);
    glutDisplayFunc(RenderScene);
    glutMainLoop();	//http://www.programmer-club.com.tw/ShowSameTitleN/opengl/2288.html
    return 0;
}
void ChangeSize(int w, int h)
{
    printf("Window Size= %d X %d\n",w,h);
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-10,10,-10,10,-10,10);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
void RenderScene(void)
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0,0,10.0f,0,0,0,0,1,0);
    // glBegin(GL_TRIANGLES);
    glBegin(GL_QUADS);

    /*
        glColor3f( 1, 0, 0);glVertex3f( 8.6603, -5, 0);
        glColor3f( 0, 1, 0);glVertex3f(      0, 10, 0);
        glColor3f( 0, 0, 1);glVertex3f(-8.6603, -5, 0);
    */

        glColor3f( 1, 0, 0); glVertex3f( 8, -8, 0);
        glColor3f( 0, 1, 0); glVertex3f( 8,  8, 0);
        glColor3f( 0, 0, 1); glVertex3f(-8,  8, 0);
        glColor3f( 1, 1, 0); glVertex3f(-8, -8, 0);

    glEnd();
    glutSwapBuffers();
}

int buildPopupMenu(void) {
    int menu;
    menu = glutCreateMenu(mainMenu);
    glutAddMenuEntry("Flat mode", MENU_FLAT);
    glutAddMenuEntry("Smooth mode", MENU_SMOOTH);
    return menu;
}

void mainMenu(int option) {
    switch (option) {
    case MENU_FLAT:
        glShadeModel(GL_FLAT);
        glutPostRedisplay();
        break;
    case MENU_SMOOTH:
        glShadeModel(GL_SMOOTH);
        glutPostRedisplay();
        break;
    default:
        break;
    }
}