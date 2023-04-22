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
    void lookLeft();
    void lookRight();
    void reset();

private:
    vec3 _eyePosition;
    vec3 _lookAtPosition;
    const vec3 _upVector;
    vec3 _lookDirection;
    vec3 _rightVector;
    const GLfloat _deltaT;
    const GLfloat _deltaR;

    void updateDirection();
    void updateRightVector();
};
