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
    _eyePosition += _lookDirection * _deltaT;
    _lookAtPosition += _lookDirection * _deltaT;
}

void myCamera::moveBackward()
{
    _eyePosition -= _lookDirection * _deltaT;
    _lookAtPosition -= _lookDirection * _deltaT;
}

void myCamera::moveLeft()
{
    _eyePosition -= _rightVector * _deltaT;
    _lookAtPosition -= _rightVector * _deltaT;
}

void myCamera::moveRight()
{
    _eyePosition += _rightVector * _deltaT;
    _lookAtPosition += _rightVector * _deltaT;
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
    _lookDirection = _lookAtPosition - _eyePosition;
    _lookDirection = _lookDirection.normalize();
}

void myCamera::updateRightVector()
{
    _rightVector = _lookDirection.cross(_upVector);
    _rightVector = _rightVector.normalize();
}
