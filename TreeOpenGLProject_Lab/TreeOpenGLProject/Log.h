#pragma once
#include <iostream>

class Log
{
public:
    static void Info(const std::string& message)
    {
        std::cout << "[INFO] : " << message << std::endl;
    }

    static void Error(const std::string& message)
    {
        std::cout << "\033[91m[ERROR]\033[0m : " << message << std::endl;
    }
};

