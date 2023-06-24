#include <iostream>
#include <array>
#include <random>
#include <cmath>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "GL/freeglut.h"
#include "glframe.h"

#define MILLISECOND_PER_FRAME 16
#define MILLISECOND_PER_TICK 20

// ~~~key map~~~
// Reset button:
// [r] : reset animation to beginning
//
// Play button:
// [SPACE] : start play the animation
//
// ~~~key map~~~

// 避免重複執行TimerFunction
bool isPlaying = false;

// These values need to be available globally
// Light values and coordinates
const GLfloat light_ambient[] = { 0.12f, 0.12f, 0.2f, 1.0f };
const GLfloat light_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat light_specular[] = { 0.9f, 0.9f, 0.9f };
const GLfloat light_position[] = { -75.0f, 150.0f, 70.0f, 0.0f };
const GLfloat specref[] = { 1.0f, 1.0f, 1.0f, 1.0f };

// Transformation matrix to project shadow
M3DMatrix44f shadowMat;

void RenderScene()
{
    // Clear the window with current clearing color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw the ground, we do manual shading to a darker green
    // in the background to give the illusion of depth
    glBegin(GL_QUADS);
        glColor3ub(0, 32, 0); //light green ground
        glVertex3f(400.0f, -150.0f, -200.0f);
        glVertex3f(-400.0f, -150.0f, -200.0f);
        glColor3ub(0, 255, 0);  // make it in green gradient
        glVertex3f(-400.0f, -150.0f, 200.0f);
        glVertex3f(400.0f, -150.0f, 200.0f);
    glEnd();

    // Draw object
    glEnable(GL_LIGHTING);

    // 海龜與魚
    glPushMatrix();
    {
        // 海龜
        glLightfv(GL_LIGHT0, GL_POSITION, light_position);
        // TODO 海龜 translate
        // TODO 畫海龜
        // TEST
        glColor3d(1, 0.25, 0.5);
        glutSolidCube(100);

        // 繞海龜旋轉的魚
        glPushMatrix();
        {
            glLightfv(GL_LIGHT0, GL_POSITION, light_position);
            // TODO 魚 translate
            // TODO 畫魚
            // TEST
            glTranslated(200, 0, 0);
            glColor3d(1, 0.25, 0.5);
            glutSolidCube(70);
        }
        glPopMatrix();
    }
    glPopMatrix();

    // 其他物體
    glPushMatrix();
    {
        glLightfv(GL_LIGHT0, GL_POSITION, light_position);
        // TODO 其他物體 translate
        // TODO 畫其他物體
    }
    glPopMatrix();

    // Draw shadow
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);

    // 海龜與魚 shadow
    glPushMatrix();
    {
        // Multiply by shadow projection matrix
        glMultMatrixf((GLfloat*)shadowMat);
        // TODO 海龜 translate
        // TODO 畫海龜的陰影
        // TEST
        glColor3d(0, 0, 0);
        glutSolidCube(100);

        // 繞海龜旋轉的魚
        glPushMatrix();
        {
            glLightfv(GL_LIGHT0, GL_POSITION, light_position);
            // TODO 魚 translate
            // TODO 畫魚的陰影
            // TEST
            glTranslated(200, 0, 0);
            glColor3d(0, 0, 0);
            glutSolidCube(70);
        }
        glPopMatrix();
    }
    glPopMatrix();

    // 其他物體 shadow
    glPushMatrix();
    {
        // Multiply by shadow projection matrix
        glMultMatrixf((GLfloat*)shadowMat);
        glLightfv(GL_LIGHT0, GL_POSITION, light_position);
        // TODO 其他物體 translate
        // TODO 畫其他物體
    }
    glPopMatrix();

    // Draw the light source
    glPushMatrix();
    glTranslatef(light_position[0], light_position[1], light_position[2]);
    glColor3ub(255, 255, 0);
    glutSolidSphere(5.0f, 10, 10);
    glPopMatrix();

    // Restore lighting state variables
    glEnable(GL_DEPTH_TEST);

    // Display the results
    glutSwapBuffers();
}

void SetupRC()
{
    // Any three points on the ground (counter clockwise order)
    M3DVector3f points[3] = {
        {-30.0f, -149.0f, -20.0f},
        {-30.0f, -149.0f, 20.0f},
        {40.0f, -149.0f, 20.0f}
    };

    // Hidden surface removal
    // Counter clock-wise polygons face out
    glEnable(GL_DEPTH_TEST);	
    glFrontFace(GL_CCW);
    glEnable(GL_CULL_FACE);
    
    // Setup and enable light 0
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glEnable(GL_LIGHT0);

    // Enable color tracking
    glEnable(GL_COLOR_MATERIAL);

    // Set Material properties to follow glColor values
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    // All materials hereafter have full specular reflectivity
    // with a high shine
    glMaterialfv(GL_FRONT, GL_SPECULAR, specref);
    glMateriali(GL_FRONT, GL_SHININESS, 128);

    // Light blue background
    glClearColor(0.0f, 0.9f, 0.9f, 1.0f);

    // Get the plane equation from three points on the ground
    M3DVector4f vPlaneEquation;
    m3dGetPlaneEquation(vPlaneEquation, points[0], points[1], points[2]);

    // Calculate projection matrix to draw shadow on the ground
    m3dMakePlanarShadowMatrix(shadowMat, vPlaneEquation, light_position);

    glEnable(GL_NORMALIZE);
}

void ChangeSize(int w, int h)
{
    GLfloat fAspect;

    // Prevent a divide by zero
    if (h == 0)
        h = 1;

    // Set Viewport to window dimensions
    glViewport(0, 0, w, h);

    // Reset coordinate system
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    fAspect = (GLfloat)w / (GLfloat)h;
    gluPerspective(60.0f, fAspect, 200.0, 500.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Move out Z axis so we can see everything
    glTranslatef(0.0f, 0.0f, -400.0f);
}

// 更新所有物件位置
void AnimationTimerFunction(int tick)
{
    if (!isPlaying)
        // TODO 重置所有物件的狀態
        return;

    // tick counter
    tick = (tick >= 500) ? 0 : tick;
    std::cout << "AnimationTimerFunction: " << tick << std::endl;

    // TODO 對海龜位置做增量
    // TODO 對魚位置做增量

    glutTimerFunc(MILLISECOND_PER_TICK, AnimationTimerFunction, ++tick);
}

// 根據最新物件位置資訊更新畫面
void TimerFunction(int value)
{
    // Redraw the scene with new coordinates
    glutPostRedisplay();
    glutTimerFunc(MILLISECOND_PER_FRAME, TimerFunction, value);
}

void Keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'r':
        // 重置所有物件位置
        std::cout << "[info] Key [R] pressed.\n";
        isPlaying = false;
        break;
    case ' ':
        // 開始播放動畫
        std::cout << "[info] Key [SPACE] pressed.\n";
        if (isPlaying)
            std::cout << "[info] Animation is already playing.\n";
        else
        {
            isPlaying = true;
            glutTimerFunc(MILLISECOND_PER_TICK, AnimationTimerFunction, 0);
        }
        break;
    default:
        break;
    }
    glutPostRedisplay();
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(1280, 720);
    glutCreateWindow("CG Final Project Animation wuhu~");
    glutReshapeFunc(ChangeSize);
    glutDisplayFunc(RenderScene);
    glutKeyboardFunc(Keyboard);
    SetupRC();
    glutTimerFunc(32, TimerFunction, 1);
    glutMainLoop();
    return 0;
}
