#include "GL\freeglut.h" // freeglut
#include "myCamera.h"
#include "myMatrix.h"

myCamera::myCamera() :
    _eyePosition{ 0.0f, 0.0f, 10.0f },
    _lookAtPosition{ 0.0f, 0.0f, 0.0f },
    _upVector{ 0.0f, 1.0f, 0.0f },
    _lookDirection{ 0.0f, 0.0f, -10.0f },
    _rightVector{ 1.0f, 0.0f, 0.0f },
    _deltaT(0.3f),
    _deltaR(5.0f)
{
    turnLeftMatrix.setRotateMatrix(_deltaR, 0, 1, 0);
    turnRightMatrix.setRotateMatrix(-_deltaR, 0, 1, 0);
    turnUpMatrix.setRotateMatrix(_deltaR, 1, 1, 0);
    turnDownMatrix.setRotateMatrix(-_deltaR, 1, 1, 0);
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
    _eyePosition += _lookDirection.normalize() * _deltaT;
    updateLookAt();
}

void myCamera::moveBackward()
{
    _eyePosition -= _lookDirection.normalize() * _deltaT;
    updateLookAt();
}

void myCamera::moveLeft()
{
    _eyePosition -= _rightVector * _deltaT;
    updateLookAt();
}

void myCamera::moveRight()
{
    _eyePosition += _rightVector * _deltaT;
    updateLookAt();
}

void myCamera::lookLeft()
{
    _lookDirection = turnLeftMatrix.mult(_lookDirection);

    updateLookAt();
    updateRightVector();
    updateUpDownMatrix();
}

void myCamera::lookRight()
{
    _lookDirection = turnRightMatrix.mult(_lookDirection);

    updateLookAt();
    updateRightVector();
    updateUpDownMatrix();
}

void myCamera::reset()
{
    _eyePosition = { 0.0f, 0.0f, 10.0f };
    _lookDirection = { 0.0f, 0.0f, -10.0f };
    updateLookAt();
    updateRightVector();
    updateUpDownMatrix();
}

void myCamera::updateLookAt()
{
    _lookAtPosition = _eyePosition + _lookDirection;
}

void myCamera::updateRightVector()
{
    _rightVector = _lookDirection.cross(_upVector);
    _rightVector = _rightVector.normalize();
}

void myCamera::updateUpDownMatrix()
{
    turnUpMatrix.setRotateMatrix(_deltaR, _rightVector.x, _rightVector.y, _rightVector.z);
    turnDownMatrix.setRotateMatrix(-_deltaR, _rightVector.x, _rightVector.y, _rightVector.z);
}
