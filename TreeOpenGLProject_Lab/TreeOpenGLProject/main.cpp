#include <iostream>
#include <array>
#include <random>
#include <cmath>
#include "GL/freeglut.h"
#include "glframe.h"

#include "Object.h"
#include "Log.h"

#define MILLISECOND_PER_FRAME 20

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
GLfloat light_position[] = { -75.0f, 150.0f, 70.0f, 0.0f };
const GLfloat specref[] = { 1.0f, 1.0f, 1.0f, 1.0f };

// Transformation matrix to project shadow
M3DMatrix44f shadowMat;

// 物件模型
Object ObjectA("Sea_Turtle");
Object ObjectB("fish");
Object ObjectC("crab");

void RenderScene()
{
    // Clear the window with current clearing color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Set Material properties to follow glColor values
    // in the background to give the illusion of depth
    glBegin(GL_QUADS);
        glColor3ub(0, 32, 0); //light green ground
        glVertex3f(400.0f, -200.0f, -200.0f);
        glVertex3f(-400.0f, -200.0f, -200.0f);
        glColor3ub(0, 255, 0);  // make it in green gradient
        glVertex3f(-400.0f, -200.0f, 200.0f);
        glVertex3f(400.0f, -200.0f, 200.0f);
    glEnd();

    // Draw object
    glEnable(GL_LIGHTING);

    // 海龜與魚
    glPushMatrix();
    {
        // 海龜
        glLightfv(GL_LIGHT0, GL_POSITION, light_position);
        ObjectA.Draw();

        // 繞海龜旋轉的魚
        glPushMatrix();
        {
            glLightfv(GL_LIGHT0, GL_POSITION, light_position);
            ObjectB.Draw();
        }
        glPopMatrix();
    }
    glPopMatrix();

    // 其他物體
    glPushMatrix();
    {
        glLightfv(GL_LIGHT0, GL_POSITION, light_position);
        ObjectC.Draw();
    }
    glPopMatrix();

    // Draw shadow
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);

    // 海龜與魚 shadow
    glPushMatrix();
    {
        // Multiply by shadow projection matrix
        glMultMatrixf(shadowMat);
        ObjectA.Draw(true);

        // 繞海龜旋轉的魚
        glPushMatrix();
        {
            glLightfv(GL_LIGHT0, GL_POSITION, light_position);
            ObjectB.Draw(true);
        }
        glPopMatrix();
    }
    glPopMatrix();

    // 其他物體 shadow
    glPushMatrix();
    {
        // Multiply by shadow projection matrix
        glMultMatrixf(shadowMat);
        glLightfv(GL_LIGHT0, GL_POSITION, light_position);
        ObjectC.Draw(true);
    }
    glPopMatrix();

    // Draw the light source
    //glPushMatrix();
    //glTranslatef(light_position[0], light_position[1], light_position[2]);
    //glColor3ub(255, 255, 0);
    //glutSolidSphere(5.0f, 10, 10);
    //glPopMatrix();

    // Restore lighting state variables
    glEnable(GL_DEPTH_TEST);

    // Display the results
    glutSwapBuffers();
}

void SetupRC()
{
    // Any three points on the ground (counter clockwise order)
    M3DVector3f points[3] = {
        {-30.0f, -200.0f, -20.0f},
        {-30.0f, -200.0f, 20.0f},
        {40.0f, -200.0f, 20.0f}
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

    // 載入每個物件的 object
    ObjectA.LoadObject();
    ObjectB.LoadObject();
    ObjectC.LoadObject();
    //ObjectC.LoadObject();
    // 載入每個物件的 texture
    ObjectA.LoadTexture();
    ObjectB.LoadTexture();
    ObjectC.LoadTexture();

    // 調整物件
    ObjectA.SetScale(3);
    ObjectB.SetScale(3);
    ObjectC.SetScale(3);

    // 設定起始位置
    ObjectA.SetPosition({ 0,0,0 });
    ObjectA.SetRotationDeg({ -90,0,-30 });
    ObjectB.SetPosition({ 0,0,0 });
    ObjectB.SetRotationDeg({ -90,0,-30 });
    ObjectC.SetPosition({ 0,0,-100 });
    ObjectC.SetRotationDeg({ 0,0,0 });
}

void ShutdownRC(void)
{
    // 刪除每個物件的 texture
    ObjectA.DeleteTexture();
    ObjectB.DeleteTexture();
    ObjectC.DeleteTexture();
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
    gluPerspective(60.0f, fAspect, 200.0, 3000.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Move out Z axis so we can see everything
    glTranslatef(0.0f, 0.0f, -400.0f);
}

// 更新所有物件位置並更新畫面 50fps
void AnimationTimerFunction(int tick)
{
    if (!isPlaying)
    {
        ObjectA.SetPosition({ 0,0,0 });
        ObjectA.SetRotationDeg({ -90,0,-30 });
        ObjectB.SetPosition({ 0,0,0 });
        ObjectB.SetRotationDeg({ -90,0,-30 });
        ObjectC.SetPosition({ 0,0,-100 });
        ObjectC.SetRotationDeg({ 0,0,0 });
    	return;
    }

    // 物件動畫 ????
    constexpr double d_deg = 120.0 / 50;
    constexpr double d_rad = m3dDegToRad(d_deg);
    constexpr double rotate_radius = 100;
    M3DVector3d a_pos = {
    cos(0.1 * tick) * -100,
    cos(0.5 * tick) * 10,
    cos(0.1 * tick) * -100
    };
    M3DVector3d a_rot = {
    d_deg * tick,
    0,
    0
    };
    ObjectA.SetPosition(a_pos);
    ObjectA.SetRotationDeg(a_rot);

    M3DVector3d b_pos = {
        cos(d_rad * tick) * rotate_radius,
        cos(0.12 * tick) * 20,
        sin(d_rad * tick) * rotate_radius
    };
    M3DVector3d b_rot = {
        0,
        -d_deg * tick + M3D_PI/2,
        0
    };
    ObjectB.SetPosition(b_pos);
    ObjectB.SetRotationDeg(b_rot);

    M3DVector3d c_pos = {
    cos(d_rad * tick*5)* 100,
    sin(d_rad * tick*3)* 100,
    -100
    };
    M3DVector3d c_rot = {
        180,
        0,
        d_deg * tick
    };
    ObjectC.SetPosition(c_pos);
    ObjectC.SetRotationDeg(c_rot);

    glutPostRedisplay();
    glutTimerFunc(MILLISECOND_PER_FRAME, AnimationTimerFunction, ++tick);
}

void Keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'r':
        // 重置所有物件位置
        Log::Info("Key [R] pressed.");
        isPlaying = false;
        break;
    case ' ':
        // 開始播放動畫
        Log::Info("Key [SPACE] pressed.");
        if (isPlaying)
            Log::Error("Animation is already playing!!");
        else
        {
            isPlaying = true;
            glutTimerFunc(MILLISECOND_PER_FRAME, AnimationTimerFunction, 0);
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
    glutMainLoop();
    ShutdownRC();
    return 0;
}
