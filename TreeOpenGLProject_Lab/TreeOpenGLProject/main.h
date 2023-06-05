#pragma once
#include <vector>
#include <array>

using Vec2i = std::array<int, 2>;
class Vertex
{
public:
    Vertex() = default;
    Vertex(int x, int y) : x(static_cast<float>(x)), y(static_cast<float>(y)) {}
    Vertex(int x, int y, float r, float g, float b) : x(static_cast<float>(x)), y(static_cast<float>(y)), r(r), g(g), b(b) {}
    Vertex(float x, float y) : x(x), y(y) {}
    Vertex(float x, float y, float r, float g, float b) : x(x), y(y), r(r), g(g), b(b) {}
    float x = 0.0f;
    float y = 0.0f;
    float r = 1.0f;
    float g = 0.25f;
    float b = 0.5f;
};

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
int getRegion(const Vec2i& v1, const Vec2i& v2);
void midpointAlgorithm(int x1, int y1, int x2, int y2);
void halfSpaceTest(const Vec2i& v1, const Vec2i& v2, const Vec2i& v3);
void pushToVerticesList(const Vertex& v);
void pushToPixelRenderQueue(const Vertex& v);
float getRandomNumber();

// crowbar
void crow(const std::vector<Vertex>& v_list, int v_num);
void scanY(const std::vector<Vertex>& v_list, int v_num, int v_index);
void scanX(Vertex& left_edge, Vertex& right_edge, int y);
void difference(const Vertex& v1, const Vertex& v2, Vertex& edge, Vertex& delta_edge, float distance, float fix);
void differenceY(const Vertex& v1, const Vertex& v2, Vertex& edge, Vertex& delta_edge, int y);
void differenceX(const Vertex& v1, const Vertex& v2, Vertex& edge, Vertex& delta_edge, int x);
void increment(Vertex& edge, const Vertex& delta);

// draw edge with color
void drawEdgeMidpointAlgorithm(Vertex v1, Vertex v2);

// animation
void myTimer(int index);

// message printer
void printVertexPixelCoordinate(const Vec2i& vertex, const std::string& name);
void printLinePixelCoordinate(const Vec2i& vertex, bool isE);
void printLineRegion(const Vec2i& v1, const Vec2i& v2, const std::string& name);

// for popup menu
void setGridDimension(int dim);
void switchDebugMode();
void drawEdges();
void drawPolygon();
void switchCrowbarMode();
