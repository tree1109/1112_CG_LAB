#include "myObject.h"
#include <iostream>
#include <sstream>
#include <fstream>

myObject::myObject():
    _origin{0.0f, 0.0f, 0.0f},
    _vertices{},
    _faces{},
    _numVertices{0},
    _numFaces{0}
{
}

void myObject::drawObject()
{
    std::vector<vec3>::const_iterator it;
    vec3 v1 = {};
    vec3 v2 = {};
    vec3 v3 = {};

    // draw the object
    //glBegin(GL_TRIANGLES);
    //for (auto face = _faces.begin(); face != _faces.end(); ++face) {
    //    v1 = _vertices[face->v1];
    //    v2 = _vertices[face->v2];
    //    v3 = _vertices[face->v3];

    //    glColor3f(0.9f, 0.21f, 0.45f);
    //    glVertex3f(v1.x, v1.y, v1.z);
    //    glVertex3f(v2.x, v2.y, v2.z);
    //    glVertex3f(v3.x, v3.y, v3.z);
    //}
    //glEnd();
    
    // print the vertices count
}

void myObject::loadObjectFile(std::string filePath)
{
    std::cout << "[info]: Loading object file." << filePath << std::endl;

    // create local vectors to store the data then
    // add a dummy vertex to the vertices vector
    std::vector<vec3> vertices;
    std::vector<face3> faces;
    vertices.push_back({ 0.0f, 0.0f, 0.0f });

    std::ifstream objFile(filePath);
    if (!objFile.is_open()) {
        std::cout << "[error]: Could not open file: " << filePath << std::endl;
    } else {
        std::cout << "[info]: File opened successfully: " << filePath << std::endl;

        // read each line of the file
        std::string line;
        std::stringstream iss;
        while (std::getline(objFile, line)) {
            iss.str(line);
            // [TODO]: behavior of stringstream is weird, need to figure out why it's not working as expected

            //if (iss.peek() == 'v') {
            //    vec3 vertex = {};
            //    iss >> vertex.x >> vertex.y >> vertex.z;
            //    vertices.push_back(vertex);
            //}
            //else if (iss.peek() == 'f') {
            //    face3 face = {};
            //    iss >> face.v1 >> face.v2 >> face.v3;
            //    faces.push_back(face);
            //}

        }
        objFile.close();
    }

    // copy data to member variables
    _vertices = vertices;
    _faces = faces;
}
