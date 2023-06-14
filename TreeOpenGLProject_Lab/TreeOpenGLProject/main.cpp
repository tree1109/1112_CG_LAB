#include <iostream>
#include <iomanip>
#include <random>
#include <array>
#include "GL/freeglut.h" // freeglut
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#define X .525731112119133606
#define Z .850650808352039932
static std::array<std::array<GLfloat, 3>, 12> vdata = {{
    {-X, 0.0, Z}, {X, 0.0, Z}, {-X, 0.0, -Z}, {X, 0.0, -Z},
    {0.0, Z, X}, {0.0, Z, -X}, {0.0, -Z, X}, {0.0, -Z, -X},
    {Z, X, 0.0}, {-Z, X, 0.0}, {Z, -X, 0.0}, {-Z, -X, 0.0}
}};

static std::array<std::array<GLuint, 3>, 20> triangles = {{
    {1,4,0}, {4,9,0}, {4,9,5}, {8,5,4}, {1,8,4},
    {1,10,8}, {10,3,8}, {8,3,5}, {3,2,5}, {3,7,2},
    {3,10,7}, {10,6,7}, {6,11,7}, {6,0,11}, {6,1,0},
    {10,1,6}, {11,0,9}, {2,11,9}, {5,2,9}, {11,2,7}
}};

GLfloat material_color[] = { 0.8f, 0.2f, 0.4f };


void MyKeyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'r':
        std::cout << "You click r\n";
        break;
    default:
        break;
    }

    glutPostRedisplay();
}

// Called to draw scene
void RenderScene(void)
{
    const int WIDTH = glutGet(GLUT_WINDOW_WIDTH);
    const int HEIGHT = glutGet(GLUT_WINDOW_HEIGHT);

    // Black background
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Clear the window with current clearing color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_NORMALIZE);

    // Flat
    glViewport(0, 0, WIDTH / 3, HEIGHT);

    glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 0.25f, 0.5f);
    for (auto& tri_vertexs : triangles)
    {
        for (auto& vertex_index: tri_vertexs)
        {
            auto& v = vdata.at(vertex_index);
            glVertex3f(v.at(0), v.at(1), v.at(2));
        }
    }
    glEnd();


    // Interpolate
    glViewport(WIDTH / 3, 0, WIDTH / 3, HEIGHT);

    // Subdivide
    glViewport(WIDTH / 3 * 2, 0, WIDTH / 3, HEIGHT);


    // Flush drawing commands
    glutSwapBuffers();
}

// This function does any needed initialization on the rendering
// context. 
void SetupRC()
{
    // Lighting data
    GLfloat light_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat light_diffuse[] = {0.7f, 0.7f, 0.7f, 1.0f};
    GLfloat light_specular[] = {0.9f, 0.9f, 0.9f};
    GLfloat light_position[] = { -80.0f, 120.0f, 100.0f, 0.0f };

    glEnable(GL_DEPTH_TEST);
    glFrontFace(GL_CCW);
    glEnable(GL_CULL_FACE);

    // Enable lighting
    glEnable(GL_LIGHTING);
    // Setup and enable light 0
    glEnable(GL_LIGHT0);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_DIFFUSE);
}

void ChangeSize(int w, int h)
{
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(1200, 400);
    glutCreateWindow("Lab12");
    SetupRC();
    glutReshapeFunc(ChangeSize);
    glutDisplayFunc(RenderScene);
    glutKeyboardFunc(MyKeyboard);

    glutMainLoop();
    return 0;
}
