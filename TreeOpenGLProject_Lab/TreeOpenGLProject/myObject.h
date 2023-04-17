#pragma once
#include <iostream>
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
	std::vector<vec3> vertices;
	std::vector<face3> faces;
};
