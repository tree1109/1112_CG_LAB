#pragma once

// function prototypes
void ChangeSize(int w, int h);
void RenderScene();
void SetupRC();
void myKeyboard(unsigned char key, int x, int y);
void myMouse(int button, int state, int x, int y);
void myMotion(int x, int y);
void printMouseWindowCoordinate(int x, int y, bool isDown);
void setVertex(int x, int y);
void linePainter(int x1, int y1, int x2, int y2);
int getRegion(int x1, int y1, int x2, int y2);
void midpointAlgorithm(int x1, int y1, int x2, int y2);

// for popup menu
void setGridDimension(int dim);
