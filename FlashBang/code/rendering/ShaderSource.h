#pragma once

#include <string>
using std::string;

class ShaderSource
{
public:
    ShaderSource();
    virtual ~ShaderSource();
    string ReadShaderFromFile(string path);
};

