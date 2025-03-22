#pragma once

#include "Math.h"

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
    Vector3 _eyePosition;
    Vector3 _lookDirection;
    const Vector3 _upVector;
    Vector3 _rightVector;
    const GLfloat _deltaT;
};
