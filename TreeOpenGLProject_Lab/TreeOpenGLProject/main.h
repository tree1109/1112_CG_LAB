#pragma once
#include "myObject.h"

enum class OBJECT {
    TEAPOT,
    TEDDY,
    OCTAHEDRON,
    GOURD
};

extern OBJECT currentObject;
extern RENDER_MODE currentRenderMode;
extern COLOR_MODE currentColorMode;

// function prototypes
void ChangeSize(int, int);
void RenderScene(void);
void SetupRC(void);
void myKeyboard(unsigned char, int, int);
void mySpecialKey(int, int, int);
void myMouse(int, int, int, int);
void myInputArbitraryAxis(void);
void myDrawAxis(GLfloat);
void myDebugInfo(void);
void printMouseWindowCoordinate(int, int, bool);
