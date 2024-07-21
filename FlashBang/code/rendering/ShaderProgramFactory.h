#pragma once
#include <string>
#include <glad/glad.h>

class ShaderProgramFactory
{
public:
    static GLuint BuildShaderProgram(std::string vertSource, std::string fragSource);
};