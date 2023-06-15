#include <iostream>
#include <array>
#include "GL/freeglut.h"

#define X 0.525731112119133606f
#define Z 0.850650808352039932f

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

enum class PolygonMode { FILL, LINE } polygon_mode;

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
// Subdivide Depth (max depth is 5):
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
        polygon_mode = (polygon_mode == PolygonMode::FILL) ? PolygonMode::LINE : PolygonMode::FILL;
        std::cout << "[info] polygon mode: " << ((polygon_mode == PolygonMode::FILL) ? "Fill" : "Line") << "\n";
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
        // draw triangle
        if (polygon_mode == PolygonMode::FILL)
        {
            glNormal3fv(v1.data());
            glVertex3fv(v1.data());
            glNormal3fv(v2.data());
            glVertex3fv(v2.data());
            glNormal3fv(v3.data());
            glVertex3fv(v3.data());
        }
        // draw line
        else
        {
            // line v1v2
            glNormal3fv(v1.data());
            glVertex3fv(v1.data());
            glNormal3fv(v2.data());
            glVertex3fv(v2.data());
            // line v2v3
            glNormal3fv(v2.data());
            glVertex3fv(v2.data());
            glNormal3fv(v3.data());
            glVertex3fv(v3.data());
            // line v3v1
            glNormal3fv(v3.data());
            glVertex3fv(v3.data());
            glNormal3fv(v1.data());
            glVertex3fv(v1.data());
        }
    }
    else
    {
        // calculate 3 new vertex by v1, v2, v3
        std::array<GLfloat, 3> v12 = { v1.at(0) + v2.at(0), v1.at(1) + v2.at(1), v1.at(2) + v2.at(2) };
        std::array<GLfloat, 3> v23 = { v2.at(0) + v3.at(0), v2.at(1) + v3.at(1), v2.at(2) + v3.at(2) };
        std::array<GLfloat, 3> v31 = { v3.at(0) + v1.at(0), v3.at(1) + v1.at(1), v3.at(2) + v1.at(2) };
        // normalize v12, v23, v31
        const GLfloat v12_length = sqrtf(powf(v12.at(0), 2) + powf(v12.at(1), 2) + powf(v12.at(2), 2));
        const GLfloat v23_length = sqrtf(powf(v23.at(0), 2) + powf(v23.at(1), 2) + powf(v23.at(2), 2));
        const GLfloat v31_length = sqrtf(powf(v31.at(0), 2) + powf(v31.at(1), 2) + powf(v31.at(2), 2));
        v12 = { v12.at(0) / v12_length, v12.at(1) / v12_length, v12.at(2) / v12_length };
        v23 = { v23.at(0) / v23_length, v23.at(1) / v23_length, v23.at(2) / v23_length };
        v31 = { v31.at(0) / v31_length, v31.at(1) / v31_length, v31.at(2) / v31_length };
        // recursive call
        subdivide_triangle(remain_depth - 1, v1, v12, v31);
        subdivide_triangle(remain_depth - 1, v2, v23, v12);
        subdivide_triangle(remain_depth - 1, v3, v31, v23);
        subdivide_triangle(remain_depth - 1, v12, v23, v31);
    }
}

void drawIcosahedron(bool smooth, bool subdivide)
{
    glBegin((polygon_mode == PolygonMode::FILL) ? GL_TRIANGLES : GL_LINES);
    // smooth shading
    if (smooth)
    {
        for (auto& triangle_vertices : TRIANGLES)
        {
            auto& v1 = V_DATA.at(triangle_vertices.at(0));
            auto& v2 = V_DATA.at(triangle_vertices.at(1));
            auto& v3 = V_DATA.at(triangle_vertices.at(2));
            if (subdivide)
                subdivide_triangle(subdivide_depth, v1, v2, v3);
            else
                subdivide_triangle(0, v1, v2, v3);
        }
    }
    // flat shading
    else
    {
        if (polygon_mode == PolygonMode::FILL)
        {
            for (auto& triangle_vertices : TRIANGLES)
            {
                auto& v1 = V_DATA.at(triangle_vertices.at(0));
                auto& v2 = V_DATA.at(triangle_vertices.at(1));
                auto& v3 = V_DATA.at(triangle_vertices.at(2));
                // calculate face normal vector
                const std::array<GLfloat, 3>& n = { v1.at(0) + v2.at(0) + v3.at(0), v1.at(1) + v2.at(1) + v3.at(1), v1.at(2) + v2.at(2) + v3.at(2) };
                // draw triangle
                glNormal3fv(n.data());
                glVertex3fv(v1.data());
                glVertex3fv(v2.data());
                glVertex3fv(v3.data());
            }
        }
        else
        {
            for (auto& triangle_vertices : TRIANGLES)
            {
                auto& v1 = V_DATA.at(triangle_vertices.at(0));
                auto& v2 = V_DATA.at(triangle_vertices.at(1));
                auto& v3 = V_DATA.at(triangle_vertices.at(2));
                // calculate line normal vector
                const std::array<GLfloat, 3>& n1 = { v1.at(0) + v2.at(0) , v1.at(1) + v2.at(1) , v1.at(2) + v2.at(2) };
                const std::array<GLfloat, 3>& n2 = { v2.at(0) + v3.at(0) , v2.at(1) + v3.at(1) , v2.at(2) + v3.at(2) };
                const std::array<GLfloat, 3>& n3 = { v3.at(0) + v1.at(0) , v3.at(1) + v1.at(1) , v3.at(2) + v1.at(2) };
                // draw line
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
    }
    glEnd();
}

void RenderScene(void)
{
    const int WIDTH = glutGet(GLUT_WINDOW_WIDTH);
    const int HEIGHT = glutGet(GLUT_WINDOW_HEIGHT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Black background
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Clear the window with current clearing color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_NORMALIZE);

    // Color
    glColor3fv(material_color.data());

    // Rotate
    glRotatef(rotate_angle_x, 1.0f, 0.0f, 0.0f);
    glRotatef(rotate_angle_y, 0.0f, 1.0f, 0.0f);
    
    // Viewport 1 : Flat
    glViewport(0, 0, WIDTH / 3, HEIGHT);
    glShadeModel(GL_FLAT);
    drawIcosahedron(false, true);

    // Viewport 2 : Interpolate
    glViewport(WIDTH / 3, 0, WIDTH / 3, HEIGHT);
    glShadeModel(GL_SMOOTH);
    drawIcosahedron(true, false);

    // Viewport 3 : Subdivide
    glViewport(WIDTH / 3 * 2, 0, WIDTH / 3, HEIGHT);
    glShadeModel(GL_SMOOTH);
    drawIcosahedron(true, true);

    // Flush drawing commands
    glutSwapBuffers();
}

// This function does any needed initialization on the rendering
// context.
void SetupRC()
{
    // Lighting data
    const GLfloat light_ambient[] = { 0.12f, 0.12f, 0.2f, 1.0f };
    const GLfloat light_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    const GLfloat light_specular[] = { 0.9f, 0.9f, 0.9f };
    const GLfloat light_position[] = { 10.0f, 10.0f, -10.0f, 0.0f };

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

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
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.1, 1.1, -1.1, 1.1, 1.1, -1.1);
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(1200, 400);
    glutCreateWindow("Lab12 :3");
    SetupRC();
    glutReshapeFunc(ChangeSize);
    glutDisplayFunc(RenderScene);
    glutKeyboardFunc(MyKeyboard);
    glutSpecialFunc(MySpecialKey);

    glutMainLoop();
    return 0;
}
