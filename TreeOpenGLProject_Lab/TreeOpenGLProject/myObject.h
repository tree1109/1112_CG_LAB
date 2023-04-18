#pragma once
#include <string>
#include <vector>
#include "GL\freeglut.h" // freeglut

struct vec3
{
	GLfloat x;
	GLfloat y;
	GLfloat z;
};
struct face3
{
	int v1;
	int v2;
	int v3;
};

class myObject
{
public:
	myObject();
	void drawObject();
	void loadObjectFile(std::string filename);

private:
	vec3 _origin;
	std::vector<vec3> _vertices;
	std::vector<face3> _faces;
	int _numVertices;
	int _numFaces;

};

