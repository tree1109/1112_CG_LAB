#include "myObject.h"
#include <iostream>
#include <fstream>

myObject::myObject():
    origin{0.0f, 0.0f, 0.0f},
    vertices{},
    faces{}
{
}

void myObject::drawObject()
{
}

void myObject::loadObjectFile(std::string filename)
{
    std::cout << "Loading object file: " << filename << std::endl;

    std::ifstream objFile(filename);
    // check file is open sucess
    if (objFile.is_open())
    {
		std::cout << "File is open" << std::endl;

        // read file line by line
        std::string line;
        while (std::getline(objFile, line))
        {
			std::cout << line << std::endl;
        }
        objFile.close();
	}
    else
    {
		std::cout << "File is not open" << std::endl;
	}

}
