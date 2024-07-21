#pragma once

#include <glad/glad.h>

class ProjectionOnlyShader
{
public:
    ProjectionOnlyShader();
    virtual ~ProjectionOnlyShader();
    GLuint GetProgramHandle();
    int Setup();

private:
    GLuint _programHandle;
};

