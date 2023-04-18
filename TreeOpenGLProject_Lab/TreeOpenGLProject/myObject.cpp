#include "myObject.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <random>

myObject::myObject() :
    _vertices{},
    _faces{},
    _renderMode(RENDER_MODE::FACES),
    _colorMode(COLOR_MODE::SINGLE),
    _pointSize(2.0f),
    _lineWidth(1.0f),
    _color{ 0.9f, 0.21f, 0.45f }
{
}

void myObject::drawObject()
{
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
        std::cout << "[error] : Invalid draw mode." << std::endl;
        break;
    }
}

void myObject::loadObjectFile(std::string filePath)
{
    std::cout << "[info] : Loading object file." << filePath << std::endl;

    // create local vectors to store the data then
    // add a dummy vertex to the vertices vector
    std::vector<vec3> vertices;
    std::vector<face3> faces;
    vertices.push_back({ 0.0f, 0.0f, 0.0f });

    std::ifstream objFile(filePath);
    if (!objFile.is_open()) {
        std::cout << "[error] : Could not open file: " << filePath << std::endl;
    } else {
        std::cout << "[info] : File opened successfully: " << filePath << std::endl;

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

void myObject::drawPoints()
{
    glPointSize(_pointSize);

    glBegin(GL_POINTS);
    for (auto v = _vertices.begin(); v != _vertices.end(); ++v) {
        fillColor();
        glVertex3f(v->x, v->y, v->z);
    }
    glEnd();
}

void myObject::drawLines()
{
    glLineWidth(_lineWidth);

    for (auto face = _faces.begin(); face != _faces.end(); ++face) {
        vec3 v1 = _vertices[face->v1];
        vec3 v2 = _vertices[face->v2];
        vec3 v3 = _vertices[face->v3];

        glBegin(GL_LINE_LOOP);
        fillColor();
        glVertex3f(v1.x, v1.y, v1.z);
        glVertex3f(v2.x, v2.y, v2.z);
        glVertex3f(v3.x, v3.y, v3.z);
        glEnd();
    }
}

void myObject::drawFaces()
{
    for (auto face = _faces.begin(); face != _faces.end(); ++face) {
        vec3 v1 = _vertices[face->v1];
        vec3 v2 = _vertices[face->v2];
        vec3 v3 = _vertices[face->v3];

        glBegin(GL_TRIANGLES);
        fillColor();
        glVertex3f(v1.x, v1.y, v1.z);
        glVertex3f(v2.x, v2.y, v2.z);
        glVertex3f(v3.x, v3.y, v3.z);
        glEnd();
    }
}

void myObject::fillColor()
{
    // set the color of the object
	glColor3f(_color.x, _color.y, _color.z);

    // [TODO] : according color mode, use random or single color
}
