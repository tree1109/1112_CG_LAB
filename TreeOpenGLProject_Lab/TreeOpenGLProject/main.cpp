#include <iostream>
#include <array>
#include "GL/freeglut.h"
#include "glframe.h"
#include "gltools.h"

// ~~~key map~~~
// Reset button:
// [r] : reset animation to beginning
//
// Play button:
// [SPACE] : start play the animation
//
// ~~~key map~~~

void MyKeyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'r':
        // reset 
        std::cout << "[info] Key [R] pressed.\n";
        break;
    case ' ':
        // reset 
        std::cout << "[info] Key [SPACE] pressed.\n";
        break;
    default:
        break;
    }
    glutPostRedisplay();
}

void RenderScene()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // TODO do something :3

    // Flush drawing commands
    glutSwapBuffers();
}

void SetupRenderingContext()
{
    // Lighting stuff
    glEnable(GL_LIGHTING);
    // Light 0 data
    const GLfloat light_ambient[] = { 0.12f, 0.12f, 0.2f, 1.0f };
    const GLfloat light_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    const GLfloat light_specular[] = { 0.9f, 0.9f, 0.9f };
    const GLfloat light_position[] = { 10.0f, 10.0f, -10.0f, 0.0f };
    // Light 0 settings
    glEnable(GL_LIGHT0);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    // Enable color tracking
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_DIFFUSE);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(1280, 720);
    glutCreateWindow("CG Final Project Animation wuhu~");
    SetupRenderingContext();
    glutDisplayFunc(RenderScene);
    glutKeyboardFunc(MyKeyboard);
    glutMainLoop();
    return 0;
}
