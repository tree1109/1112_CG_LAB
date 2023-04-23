#pragma once
#include "myObject.h"

enum class OBJECT {
    USER_IMPORTED,
    TEAPOT,
    TEDDY,
    OCTAHEDRON,
    GOURD
};

extern RENDER_MODE currentRenderMode;
extern COLOR_MODE currentColorMode;

// function prototypes
void ChangeSize(int w, int h);
void RenderScene();
void SetupRC();
void myKeyboard(unsigned char key, int x, int y);
void mySpecialKey(int key, int x, int y);
void myMouse(int button, int state, int x, int y);
void myMotion(int x, int y);
void myInputArbitraryAxis();
void myInputObjectFilePath();
void myDrawAxis(GLfloat length);
void myDebugInfo();
void printMouseWindowCoordinate(int x, int y, bool isDown);

// call by myPopupMenu
void setCurrentObject(OBJECT seletedObj);
