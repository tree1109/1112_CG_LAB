#pragma once
#include "vec3.h"

class myCamera
{
public:
    myCamera();
    void work();
    void moveForward();
    void moveBackward();
    void moveLeft();
    void moveRight();
    void setViewAngle(GLfloat pitch, GLfloat yaw);
    void reset();

private:
    vec3 _eyePosition;
    vec3 _lookDirection;
    const vec3 _upVector;
    vec3 _rightVector;
    const GLfloat _deltaT;
};
