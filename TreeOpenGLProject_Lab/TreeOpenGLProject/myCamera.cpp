#include <cmath>
#include "GL\freeglut.h" // freeglut
#include "myCamera.h"

myCamera::myCamera() :
    _eyePosition{ 0.0f, 0.0f, 10.0f },
    _lookAtPosition{ 0.0f, 0.0f, 0.0f },
    _upVector{ 0.0f, 1.0f, 0.0f },
    _lookDirection{ 0.0f, 0.0f, -1.0f },
    _rightVector{ 1.0f, 0.0f, 0.0f },
    _deltaT(0.3f),
    _deltaR(5.0f)
{
}

void myCamera::work()
{
    gluLookAt(
        _eyePosition.x, _eyePosition.y, _eyePosition.z,
        _lookAtPosition.x, _lookAtPosition.y, _lookAtPosition.z,
        _upVector.x, _upVector.y, _upVector.z);
}

void myCamera::moveForward()
{
    _eyePosition.x += _lookDirection.x * _deltaT;
    _eyePosition.y += _lookDirection.y * _deltaT;
    _eyePosition.z += _lookDirection.z * _deltaT;
    _lookAtPosition.x += _lookDirection.x * _deltaT;
    _lookAtPosition.y += _lookDirection.y * _deltaT;
    _lookAtPosition.z += _lookDirection.z * _deltaT;
}

void myCamera::moveBackward()
{
    _eyePosition.x -= _lookDirection.x * _deltaT;
    _eyePosition.y -= _lookDirection.y * _deltaT;
    _eyePosition.z -= _lookDirection.z * _deltaT;
    _lookAtPosition.x -= _lookDirection.x * _deltaT;
    _lookAtPosition.y -= _lookDirection.y * _deltaT;
    _lookAtPosition.z -= _lookDirection.z * _deltaT;
}

void myCamera::moveLeft()
{
    _eyePosition.x -= _rightVector.x * _deltaT;
    _eyePosition.y -= _rightVector.y * _deltaT;
    _eyePosition.z -= _rightVector.z * _deltaT;
    _lookAtPosition.x -= _rightVector.x * _deltaT;
    _lookAtPosition.y -= _rightVector.y * _deltaT;
    _lookAtPosition.z -= _rightVector.z * _deltaT;
}

void myCamera::moveRight()
{
    _eyePosition.x += _rightVector.x * _deltaT;
    _eyePosition.y += _rightVector.y * _deltaT;
    _eyePosition.z += _rightVector.z * _deltaT;
    _lookAtPosition.x += _rightVector.x * _deltaT;
    _lookAtPosition.y += _rightVector.y * _deltaT;
    _lookAtPosition.z += _rightVector.z * _deltaT;
}

void myCamera::lookLeft()
{
    // TODO: rotate lookAtPosition around eyePosition
}

void myCamera::lookRight()
{
    // TODO: rotate lookAtPosition around eyePosition
}

void myCamera::reset()
{
    _eyePosition = { 0.0f, 0.0f, 10.0f };
    _lookAtPosition = { 0.0f, 0.0f, 0.0f };
    _lookDirection = { 0.0f, 0.0f, -1.0f };
    _rightVector = { 1.0f, 0.0f, 0.0f };
}

void myCamera::updateDirection()
{
    vec3 normal = {
        _lookAtPosition.x - _eyePosition.x,
        _lookAtPosition.y - _eyePosition.y,
        _lookAtPosition.z - _eyePosition.z, };
    GLfloat length = (GLfloat)sqrt(pow(normal.x, 2) + pow(normal.y, 2) + pow(normal.z, 2));

    normal.x /= length;
    normal.y /= length;
    normal.z /= length;

    _lookDirection = normal;
}

void myCamera::updateRightVector()
{
    // cross product
    vec3 normal = {
        _lookDirection.y * _upVector.z - _lookDirection.z * _upVector.y,
        _lookDirection.z * _upVector.x - _lookDirection.x * _upVector.z,
        _lookDirection.x * _upVector.y - _lookDirection.y * _upVector.x
    };
    GLfloat length = (GLfloat)sqrt(pow(normal.x, 2) + pow(normal.y, 2) + pow(normal.z, 2));

    normal.x /= length;
    normal.y /= length;
    normal.z /= length;

    _rightVector = normal;
}
