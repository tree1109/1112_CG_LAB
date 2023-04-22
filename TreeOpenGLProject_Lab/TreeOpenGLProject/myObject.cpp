#include <iostream>
#include <sstream>
#include <fstream>
#include <random>
#include "myObject.h"

myObject::myObject() :
    _vertices{},
    _faces{},
    _renderMode(RENDER_MODE::FACES),
    _colorMode(COLOR_MODE::SINGLE),
    _pointSize(5.0f),
    _lineWidth(1.0f),
    _color{ 0.9f, 0.21f, 0.45f },
    _boundingBox{},
    _position{ 0.0f, 0.0f, 0.0f },
    _rotation{ 0.0f, 0.0f, 0.0f },
    _scaling(1.0f),
    _transformationMatrix(),
    _arbitraryTheta(0.0f),
    _v1{ -5, -5, -5 },
    _v2{ 5, 5, 5 }
{
}

void myObject::drawObject(bool showBoundingBox)
{
    glPointSize(_pointSize);
    glLineWidth(_lineWidth);

    if (showBoundingBox)
        drawBoundingBox();

    switch (_renderMode)
    {
    case RENDER_MODE::POINTS:
        drawPoints();
        break;
    case RENDER_MODE::LINES:
        drawLines();
        break;
    case RENDER_MODE::FACES:
        drawFaces();
        break;
    default:
        std::cout << "[error] Invalid render mode." << std::endl;
        break;
    }

    // reset
    glPointSize(1.0f);
    glLineWidth(1.0f);
}

// TODO: move out object file reader, maybe use a factory pattern i guess ? i will try try look
void myObject::loadObjectFile(std::string filePath)
{
    std::cout << "[info] Loading object file." << filePath << std::endl;

    // create local vectors to store the data then
    // add a dummy vertex to the vertices vector
    std::vector<vec3> vertices;
    std::vector<face3> faces;
    vertices.push_back({ 0.0f, 0.0f, 0.0f });

    std::ifstream objFile(filePath);
    if (!objFile.is_open()) {
        std::cout << "[error] Could not open file: " << filePath << std::endl;
    } else {
        std::cout << "[info] File opened successfully: " << filePath << std::endl;

        std::string lineBuffer;
        while (std::getline(objFile, lineBuffer)) {
            std::istringstream lineSS(lineBuffer);
            std::string token;

            // read the first token from the line
            lineSS >> token;
            if (token == "v") {
                // read the vertex data
                vec3 vertex = {};
                lineSS >> vertex.x >> vertex.y >> vertex.z;
                vertices.push_back(vertex);
            }
            else if (token == "f") {
                // read the face data
                face3 face = {};
                lineSS >> face.v1 >> face.v2 >> face.v3;
                faces.push_back(face);
            }
        }
        objFile.close();
    }

    // copy data to member variables
    _vertices = vertices;
    _faces = faces;

    // calculate the bounding box
    createBoundingBox();
}

void myObject::doTransformation()
{
    fixPositionVector();
    _transformationMatrix.doScale(_scaling);
    _transformationMatrix.doRotate(_rotation.x, 1, 0, 0);
    _transformationMatrix.doRotate(_rotation.y, 0, 1, 0);
    _transformationMatrix.doRotate(_rotation.z, 0, 0, 1);
    _transformationMatrix.doTranslate(_position.x, _position.y, _position.z);

    drawArbitraryAxis();
    _transformationMatrix.doArbitraryRotate(_arbitraryTheta, _v1, _v2);
}

void myObject::setTransformation(vec3 position, vec3 rotation, GLfloat scaling = 1.0f)
{
    _position = position;
    _rotation = rotation;
    _scaling = scaling;
}

void myObject::setRenderMode(RENDER_MODE renderMode)
{
    _renderMode = renderMode;
}

void myObject::setColorMode(COLOR_MODE colorMode)
{
    _colorMode = colorMode;
}

void myObject::setColor(GLfloat r, GLfloat g, GLfloat b)
{
    _color = { r, g, b };
}

GLfloat myObject::getScalingCoefficient()
{
    // view space is 20x20x30
    static const GLfloat factor = 0.8f;

    // get the max and min values for each axis
    GLfloat xSize = _boundingBox.xMax - _boundingBox.xMin;
    GLfloat ySize = _boundingBox.yMax - _boundingBox.yMin;
    GLfloat zSize = _boundingBox.zMax - _boundingBox.zMin;
    GLfloat xScalingCoefficient = factor * 20 / xSize;
    GLfloat yScalingCoefficient = factor * 20 / ySize;
    GLfloat zScalingCoefficient = factor * 30 / zSize;
    GLfloat result = 1;

    // return the smallest scaling coefficient
    result = xScalingCoefficient < yScalingCoefficient ? xScalingCoefficient : yScalingCoefficient;
    result = result < zScalingCoefficient ? result : zScalingCoefficient;

    return result;
}

void myObject::fitToWindow()
{
    _scaling *= getScalingCoefficient();
}

void myObject::setArbitraryRotate(GLfloat theta, vec3 v1, vec3 v2)
{
    _arbitraryTheta = theta;
    _v1 = v1;
    _v2 = v2;
}

void myObject::drawPoints()
{
    for (auto& face : _faces) {
        vec3 v1 = _vertices[face.v1];
        vec3 v2 = _vertices[face.v2];
        vec3 v3 = _vertices[face.v3];
        vec3 normal = getNormalVector(v1, v2, v3);

        glBegin(GL_POINTS);
        glNormal3f(normal.x, normal.y, normal.z);
        fillColor();
        glVertex3f(v1.x, v1.y, v1.z);
        fillColor();
        glVertex3f(v2.x, v2.y, v2.z);
        fillColor();
        glVertex3f(v3.x, v3.y, v3.z);
        glEnd();
    }
}

void myObject::drawLines()
{
    for (auto& face : _faces) {
        vec3 v1 = _vertices[face.v1];
        vec3 v2 = _vertices[face.v2];
        vec3 v3 = _vertices[face.v3];
        vec3 normal = getNormalVector(v1, v2, v3);

        glBegin(GL_LINE_LOOP);
        glNormal3f(normal.x, normal.y, normal.z);
        fillColor();
        glVertex3f(v1.x, v1.y, v1.z);
        glVertex3f(v2.x, v2.y, v2.z);
        glVertex3f(v3.x, v3.y, v3.z);
        glEnd();
    }
}

void myObject::drawFaces()
{
    for (auto &face : _faces) {
        vec3 v1 = _vertices[face.v1];
        vec3 v2 = _vertices[face.v2];
        vec3 v3 = _vertices[face.v3];
        vec3 normal = getNormalVector(v1, v2, v3);

        glBegin(GL_TRIANGLES);
        glNormal3f(normal.x, normal.y, normal.z);
        fillColor();
        glVertex3f(v1.x, v1.y, v1.z);
        glVertex3f(v2.x, v2.y, v2.z);
        glVertex3f(v3.x, v3.y, v3.z);
        glEnd();
    }
}

void myObject::fillColor()
{
    // random number generator
    // more at https://blog.gtwang.org/programming/cpp-random-number-generator-and-probability-distribution-tutorial/
    static std::random_device randomDevice;
    static std::mt19937 generator(randomDevice());
    static std::normal_distribution<double> normal(0.7, 0.4);

    switch (_colorMode)
    {
    case COLOR_MODE::SINGLE:
        glColor3f(_color.x, _color.y, _color.z);
        break;
    case COLOR_MODE::RANDOM:
        glColor3f((GLfloat)normal(generator), (GLfloat)normal(generator), (GLfloat)normal(generator));
        break;
    default:
        std::cout << "[error] Invalid color mode." << std::endl;
        break;
    }
}

void myObject::createBoundingBox()
{
    _boundingBox = {};

    // find the bounding box
    for (auto &v : _vertices) {
        _boundingBox.xMin = (v.x < _boundingBox.xMin)? v.x : _boundingBox.xMin;
        _boundingBox.xMax = (v.x > _boundingBox.xMax)? v.x : _boundingBox.xMax;
        _boundingBox.yMin = (v.y < _boundingBox.yMin)? v.y : _boundingBox.yMin;
        _boundingBox.yMax = (v.y > _boundingBox.yMax)? v.y : _boundingBox.yMax;
        _boundingBox.zMin = (v.z < _boundingBox.zMin)? v.z : _boundingBox.zMin;
        _boundingBox.zMax = (v.z > _boundingBox.zMax)? v.z : _boundingBox.zMax;
    }
}

void myObject::drawBoundingBox()
{
    // draw the bounding box
    glBegin(GL_LINES);
    // x-axis
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex3f(_boundingBox.xMin, _boundingBox.yMin, _boundingBox.zMin);
    glVertex3f(_boundingBox.xMax, _boundingBox.yMin, _boundingBox.zMin);
    glVertex3f(_boundingBox.xMin, _boundingBox.yMax, _boundingBox.zMin);
    glVertex3f(_boundingBox.xMax, _boundingBox.yMax, _boundingBox.zMin);
    glVertex3f(_boundingBox.xMin, _boundingBox.yMin, _boundingBox.zMax);
    glVertex3f(_boundingBox.xMax, _boundingBox.yMin, _boundingBox.zMax);
    glVertex3f(_boundingBox.xMin, _boundingBox.yMax, _boundingBox.zMax);
    glVertex3f(_boundingBox.xMax, _boundingBox.yMax, _boundingBox.zMax);
    // y-axis
    glVertex3f(_boundingBox.xMin, _boundingBox.yMin, _boundingBox.zMin);
    glVertex3f(_boundingBox.xMin, _boundingBox.yMax, _boundingBox.zMin);
    glVertex3f(_boundingBox.xMax, _boundingBox.yMin, _boundingBox.zMin);
    glVertex3f(_boundingBox.xMax, _boundingBox.yMax, _boundingBox.zMin);
    glVertex3f(_boundingBox.xMin, _boundingBox.yMin, _boundingBox.zMax);
    glVertex3f(_boundingBox.xMin, _boundingBox.yMax, _boundingBox.zMax);
    glVertex3f(_boundingBox.xMax, _boundingBox.yMin, _boundingBox.zMax);
    glVertex3f(_boundingBox.xMax, _boundingBox.yMax, _boundingBox.zMax);
    // z-axis
    glVertex3f(_boundingBox.xMin, _boundingBox.yMin, _boundingBox.zMin);
    glVertex3f(_boundingBox.xMin, _boundingBox.yMin, _boundingBox.zMax);
    glVertex3f(_boundingBox.xMax, _boundingBox.yMin, _boundingBox.zMin);
    glVertex3f(_boundingBox.xMax, _boundingBox.yMin, _boundingBox.zMax);
    glVertex3f(_boundingBox.xMin, _boundingBox.yMax, _boundingBox.zMin);
    glVertex3f(_boundingBox.xMin, _boundingBox.yMax, _boundingBox.zMax);
    glVertex3f(_boundingBox.xMax, _boundingBox.yMax, _boundingBox.zMin);
    glVertex3f(_boundingBox.xMax, _boundingBox.yMax, _boundingBox.zMax);
    glEnd();
}

void myObject::drawArbitraryAxis()
{
    glBegin(GL_LINES);
    // yellow
    glColor3f(1.0f, 1.0f, 0.0f);
    glVertex3f(_v1.x, _v1.y, _v1.z);
    glVertex3f(_v2.x, _v2.y, _v2.z);
    glEnd();

    drawCross(_v1);
    drawCross(_v2);
}

void myObject::drawCross(vec3 v)
{
    GLfloat length = 1 / _scaling; // cross length

    glBegin(GL_LINES);
    // red mark
    glColor3f(1, 0, 0);
    glVertex3f(v.x - length, v.y, v.z);
    glVertex3f(v.x + length, v.y, v.z);
    // green mark
    glColor3f(0, 1, 0);
    glVertex3f(v.x, v.y - length, v.z);
    glVertex3f(v.x, v.y + length, v.z);
    // blue mark
    glColor3f(0, 0, 1);
    glVertex3f(v.x, v.y, v.z - length);
    glVertex3f(v.x, v.y, v.z + length);
    glEnd();
}

void myObject::fixPositionVector()
{
    _position = { _position.x / _scaling, _position.y / _scaling, _position.z / _scaling };
    _v1 = { _v1.x / _scaling, _v1.y / _scaling, _v1.z / _scaling };
    _v2 = { _v2.x / _scaling, _v2.y / _scaling, _v2.z / _scaling };
}

vec3 myObject::getNormalVector(vec3 v1, vec3 v2, vec3 v3)
{
    vec3 u = { v3.x - v1.x, v3.y - v1.y, v3.z - v1.z };
    vec3 v = { v2.x - v1.x, v2.y - v1.y, v2.z - v1.z };
    // cross product
    vec3 normal = {
        u.y * v.z - u.z * v.y,
        u.z * v.x - u.x * v.z,
        u.x * v.y - u.y * v.x 
    };
    GLfloat length = (GLfloat)sqrt(pow(normal.x, 2) + pow(normal.y, 2) + pow(normal.z, 2));

    // normalize
    normal.x /= length;
    normal.y /= length;
    normal.z /= length;

    return normal;
}
