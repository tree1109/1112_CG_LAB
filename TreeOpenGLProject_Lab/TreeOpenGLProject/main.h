#pragma once
using Vec2i = std::array<int, 2>;
using Vec2f = std::array<float, 2>;
// TODO 新增包含x, y, r, g, b 的結構

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
void vertexPainter(const Vec2i& v, const std::string& name);
void linePainter(const Vec2i& v1, const Vec2i& v2, const std::string& name);
void facePainter(const Vec2i& v1, const Vec2i& v2, const Vec2i& v3, const std::string& name);
int getRegion(const Vec2i& v1, const Vec2i& v2);
void midpointAlgorithm(int x1, int y1, int x2, int y2);
void halfSpaceTest(const Vec2i& v1, const Vec2i& v2, const Vec2i& v3);
void pushToPixelRenderQueue(const Vec2i& v);

// crowbar
void crow(const std::vector<Vec2i>& v_list, int v_num);
void scanY(const std::vector<Vec2f>& v_list, int v_num, int v_index);
void scanX(Vec2f& left_edge, Vec2f& right_edge, int y);
void difference(const Vec2f& v1, const Vec2f& v2, Vec2f& edge, Vec2f& delta_edge, float distance, float fix);
void differenceY(const Vec2f& v1, const Vec2f& v2, Vec2f& edge, Vec2f& delta_edge, int y);
void differenceX(const Vec2f& v1, const Vec2f& v2, Vec2f& edge, Vec2f& delta_edge, int x);
void increment(Vec2f& edge,const Vec2f& delta);

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
