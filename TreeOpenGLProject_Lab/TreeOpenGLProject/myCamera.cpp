#include "GL\freeglut.h" // freeglut
#include "myCamera.h"
#include "myMatrix.h"

myCamera::myCamera() :
    _eyePosition{ 0.0f, 0.0f, 10.0f },
    _lookDirection{ 0.0f, 0.0f, -1.0f },
    _upVector{ 0.0f, 1.0f, 0.0f },
    _deltaT(0.5f)
{
}

void myCamera::work()
{
    const vec3 lookAt= _eyePosition + _lookDirection;
    gluLookAt(
        _eyePosition.x, _eyePosition.y, _eyePosition.z,
        lookAt.x, lookAt.y, lookAt.z,
        _upVector.x, _upVector.y, _upVector.z);
}

void myCamera::moveForward()
{
    _eyePosition += _lookDirection.normalize() * _deltaT;
}

void myCamera::moveBackward()
{
    _eyePosition -= _lookDirection.normalize() * _deltaT;
}

void myCamera::moveLeft()
{
    _eyePosition -= _lookDirection.cross(_upVector).normalize() * _deltaT;
}

void myCamera::moveRight()
{
    _eyePosition += _lookDirection.cross(_upVector).normalize() * _deltaT;
}

void myCamera::setViewAngle(GLfloat pitch, GLfloat yaw)
{
    static myMatrix matrix;
    matrix.setRotateMatrix(pitch, 1, 0, 0);
    _lookDirection = matrix.mult({0,0,-1});
    matrix.setRotateMatrix(yaw, 0, 1, 0);
    _lookDirection = matrix.mult(_lookDirection);
}

void myCamera::reset()
{
    _eyePosition = { 0.0f, 0.0f, 10.0f };
    _lookDirection = { 0.0f, 0.0f, -10.0f };
}
