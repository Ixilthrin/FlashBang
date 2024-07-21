#pragma once

#include <glad/glad.h>

class RotationShaderNoProjection
{
public:
    RotationShaderNoProjection();
    virtual ~RotationShaderNoProjection();
    GLuint GetProgramHandle();
    int Setup();

private:
    GLuint _programHandle;
};

