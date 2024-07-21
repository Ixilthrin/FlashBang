#include "ProjectionOnlyShader.h"

#include "ShaderSource.h"

ProjectionOnlyShader::ProjectionOnlyShader()
{
    _programHandle = 0;
}


ProjectionOnlyShader::~ProjectionOnlyShader()
{
}

GLuint ProjectionOnlyShader::GetProgramHandle()
{
    return _programHandle;
}

int ProjectionOnlyShader::Setup()
{
    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    if (vertShader == 0)
        return -4;

    ShaderSource shaderSource;
    string shaderCode = shaderSource.ReadShaderFromFile("../FlashBang/shaders/projection.vert.glsl");
    const GLchar *codeArray[] = { shaderCode.c_str() };

    glShaderSource(vertShader, 1, codeArray, NULL);

    glCompileShader(vertShader);

    GLint result;
    glGetShaderiv(vertShader, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE || shaderCode.length() < 1)
        return -5;

    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    if (fragShader == 0)
        return -6;

    string fragCode = shaderSource.ReadShaderFromFile("../FlashBang/shaders/basic.frag.glsl");
    const GLchar *fragArray[] = { fragCode.c_str() };

    glShaderSource(fragShader, 1, fragArray, NULL);

    glCompileShader(fragShader);

    GLint fragResult;
    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &fragResult);
    if (fragResult == GL_FALSE || fragCode.length() < 1)
        return -7;

    _programHandle = glCreateProgram();
    if (_programHandle == 0)
        return -8;

    glAttachShader(_programHandle, vertShader);
    glAttachShader(_programHandle, fragShader);

    glLinkProgram(_programHandle);

    GLint linkStatus;
    glGetProgramiv(_programHandle, GL_LINK_STATUS, &linkStatus);
    if (linkStatus == GL_FALSE)
        return -9;

    glUseProgram(_programHandle);

    glDetachShader(_programHandle, vertShader);
    glDetachShader(_programHandle, fragShader);

    glDeleteShader(vertShader);
    glDeleteShader(fragShader);

    return 0;
}
