#pragma once
using Vertex = std::array<int, 2>;

// FSM for setting vertex
enum class CURRENT_VERTEX
{
    V1,
    V2,
    V3
};

// function prototypes
void ChangeSize(int w, int h);
void RenderScene();
void SetupRC();
void myKeyboard(unsigned char key, int x, int y);
void myMouse(int button, int state, int x, int y);
void setVertex(int x, int y);
void vertexPainter(const Vertex& v, const std::string& name);
void linePainter(const Vertex& v1, const Vertex& v2, const std::string& name);
void facePainter(const Vertex& v1, const Vertex& v2, const Vertex& v3, const std::string& name);
int getRegion(const Vertex& v1, const Vertex& v2);
void midpointAlgorithm(int x1, int y1, int x2, int y2);
void halfSpaceTest(const Vertex& v1, const Vertex& v2, const Vertex& v3);

// animation
void myTimer(int index);

// message printer
void printVertexPixelCoordinate(const Vertex& vertex, const std::string& name);
void printLinePixelCoordinate(const Vertex& vertex, bool isE);
void printLineRegion(const Vertex& v1, const Vertex& v2, const std::string& name);

// for popup menu
void setGridDimension(int dim);
void switchDebugMode();
