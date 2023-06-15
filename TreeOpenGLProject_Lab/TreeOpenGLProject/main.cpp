#include <iostream>
#include <iomanip>
#include <random>
#include <array>
#include "GL/freeglut.h" // freeglut
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#define X .525731112119133606
#define Z .850650808352039932

const std::array<std::array<GLfloat, 3>, 12> V_DATA = {{
    {-X, 0.0f, Z}, {X, 0.0f, Z}, {-X, 0.0f, -Z}, {X, 0.0f, -Z},
    {0.0f, Z, X}, {0.0f, Z, -X}, {0.0f, -Z, X}, {0.0f, -Z, -X},
    {Z, X, 0.0f}, {-Z, X, 0.0f}, {Z, -X, 0.0f}, {-Z, -X, 0.0f}
}};

const std::array<std::array<GLuint, 3>, 20> TRIANGLES = {{
    {1,4,0}, {4,9,0}, {4,9,5}, {8,5,4}, {1,8,4},
    {1,10,8}, {10,3,8}, {8,3,5}, {3,2,5}, {3,7,2},
    {3,10,7}, {10,6,7}, {6,11,7}, {6,0,11}, {6,1,0},
    {10,1,6}, {11,0,9}, {2,11,9}, {5,2,9}, {11,2,7}
}};

const std::array<GLfloat, 3> material_color = { 1.0f, 0.25f, 0.5f };

GLfloat rotate_angle_x = 0.0f;
GLfloat rotate_angle_y = 0.0f;

enum class POLYGON_MODE { FILL, LINE } polygon_mode = POLYGON_MODE::FILL;

int subdivide_depth = 0;

// ~~~key map~~~
// [r] : reset rotate and subdivide depth
//
// Rotate:
// [↑], [↓] : rotate by x-axis
// [←], [→] : rotate by y-axis
//
// Switch polygon mode:
// [C] : switch polygon mode between Line and Fill
//
// Subdivide Depth:
// [Z] : decrease depth by 1
// [X] : increase depth by 1
//
// ~~~key map~~~

void MyKeyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'r':
        // reset rotate and subdivide depth
        rotate_angle_x = 0.0f;
        rotate_angle_y = 0.0f;
        subdivide_depth = 0;
        std::cout << "[info] reset rotate and subdivide depth\n";
        break;
    case 'c':
        // switch polygon mode between Line and Fill mode
        polygon_mode = (polygon_mode == POLYGON_MODE::FILL) ? POLYGON_MODE::LINE : POLYGON_MODE::FILL;
        std::cout << "[info] polygon mode: " << ((polygon_mode == POLYGON_MODE::FILL) ? "Fill" : "Line") << "\n";
        break;
    case 'z':
        subdivide_depth = std::max(0, subdivide_depth - 1);
        std::cout << "[info] subdivide depth: " << subdivide_depth << "\n";
        break;
    case 'x':
        subdivide_depth = std::min(5, subdivide_depth + 1);
        std::cout << "[info] subdivide depth: " << subdivide_depth << "\n";
        break;
    default:
        break;
    }
    glutPostRedisplay();
}

void MySpecialKey(int key, int x, int y)
{
    constexpr GLfloat delta_rotate = 5.0f;
    switch (key)
    {
    case GLUT_KEY_LEFT:
        rotate_angle_y += delta_rotate;
        break;
    case GLUT_KEY_RIGHT:
        rotate_angle_y -= delta_rotate;
        break;
    case GLUT_KEY_UP:
        rotate_angle_x += delta_rotate;
        break;
    case GLUT_KEY_DOWN:
        rotate_angle_x -= delta_rotate;
        break;
    default:
        break;
    }
    glutPostRedisplay();
}

void subdivide_triangle(int remain_depth, const std::array<GLfloat, 3>& v1, const std::array<GLfloat, 3>& v2, const std::array<GLfloat, 3>& v3)
{
    if (remain_depth == 0)
    {
        glNormal3fv(v1.data());
        glVertex3fv(v1.data());
        glNormal3fv(v2.data());
        glVertex3fv(v2.data());
        glNormal3fv(v3.data());
        glVertex3fv(v3.data());
    }
    else
    {
        std::array<GLfloat, 3> v4 = { v1.at(0) + v2.at(0) + v3.at(0), v1.at(1) + v2.at(1) + v3.at(1), v1.at(2) + v2.at(2) + v3.at(2) };
        // normalize v4
        GLfloat v4_length = sqrtf(v4.at(0) * v4.at(0) + v4.at(1) * v4.at(1) + v4.at(2) * v4.at(2));
        v4.at(0) /= v4_length;
        v4.at(1) /= v4_length;
        v4.at(2) /= v4_length;
        subdivide_triangle(remain_depth - 1, v4, v1, v2);
        subdivide_triangle(remain_depth - 1, v4, v2, v3);
        subdivide_triangle(remain_depth - 1, v4, v3, v1);
    }
}

void drawIcosahedron(bool smooth)
{
    if (smooth)
    {
        for (auto& tri_vertices : TRIANGLES)
        {
            glBegin((polygon_mode == POLYGON_MODE::FILL) ? GL_TRIANGLES : GL_LINE_STRIP);
            if (subdivide_depth == 0)
            {
                for (auto& vertex_index : tri_vertices)
                {
                    auto& v = V_DATA.at(vertex_index);
                    glNormal3fv(v.data());
                    glVertex3fv(v.data());
                }
            }
            else
            {
                auto& v1 = V_DATA.at(tri_vertices[0]);
                auto& v2 = V_DATA.at(tri_vertices[1]);
                auto& v3 = V_DATA.at(tri_vertices[2]);
                subdivide_triangle(subdivide_depth, v1, v2, v3);
            }

            glEnd();
        }
    }
    else
    {
        glBegin((polygon_mode == POLYGON_MODE::FILL) ? GL_TRIANGLES : GL_LINES);
        for (auto& tri_vertices : TRIANGLES)
        {
            auto& v1 = V_DATA.at(tri_vertices[0]);
            auto& v2 = V_DATA.at(tri_vertices[1]);
            auto& v3 = V_DATA.at(tri_vertices[2]);
            if (polygon_mode == POLYGON_MODE::FILL)
            {
                std::array<GLfloat, 3>&& n = { v1[0] + v2[0] + v3[0], v1[1] + v2[1] + v3[1], v1[2] + v2[2] + v3[2] };
                glNormal3fv(n.data());
                glVertex3fv(v1.data());
                glVertex3fv(v2.data());
                glVertex3fv(v3.data());
            }
            else
            {
                std::array<GLfloat, 3>&& n1 = { v1[0] + v2[0] , v1[1] + v2[1] , v1[2] + v2[2] };
                std::array<GLfloat, 3>&& n2 = { v2[0] + v3[0] , v2[1] + v3[1] , v2[2] + v3[2] };
                std::array<GLfloat, 3>&& n3 = { v3[0] + v1[0] , v3[1] + v1[1] , v3[2] + v1[2] };
                glNormal3fv(n1.data());
                glVertex3fv(v1.data());
                glVertex3fv(v2.data());
                glNormal3fv(n2.data());
                glVertex3fv(v2.data());
                glVertex3fv(v3.data());
                glNormal3fv(n3.data());
                glVertex3fv(v3.data());
                glVertex3fv(v1.data());
            }
        }
        glEnd();
    }
}

void RenderScene(void)
{
    const int WIDTH = glutGet(GLUT_WINDOW_WIDTH);
    const int HEIGHT = glutGet(GLUT_WINDOW_HEIGHT);

    // Black background
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Clear the window with current clearing color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_NORMALIZE);

    // Rotate
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotatef(rotate_angle_y, 0.0f, 1.0f, 0.0f);
    glRotatef(rotate_angle_x, 1.0f, 0.0f, 0.0f);

    // Color
    glColor3fv(material_color.data());

    // Viewport 1 : Flat
    glViewport(0, 0, WIDTH / 3, HEIGHT);
    glShadeModel(GL_FLAT);
    drawIcosahedron(false);

    // Viewport 2 : Interpolate
    glViewport(WIDTH / 3, 0, WIDTH / 3, HEIGHT);
    glShadeModel(GL_SMOOTH);
    drawIcosahedron(true);

    // Viewport 3 : Subdivide
    glViewport(WIDTH / 3 * 2, 0, WIDTH / 3, HEIGHT);
    glShadeModel(GL_SMOOTH);
    drawIcosahedron(true);

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
    GLfloat light_position[] = { 10.0f, 10.0f, -10.0f, 0.0f };

    // Enable lighting
    glEnable(GL_LIGHTING);
    // Setup and enable light 0
    glEnable(GL_LIGHT0);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    // Enable color tracking
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_DIFFUSE);
    glEnable(GL_DEPTH_TEST);
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
    glutSpecialFunc(MySpecialKey);

    glutMainLoop();
    return 0;
}
