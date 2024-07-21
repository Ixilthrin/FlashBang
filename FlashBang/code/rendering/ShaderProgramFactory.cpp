#include "ShaderProgramFactory.h"

#include "ShaderSource.h"

GLuint ShaderProgramFactory::BuildShaderProgram(std::string vertSource, std::string fragSource)
{
    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    if (vertShader == 0)
        return 0;

    ShaderSource shaderSource;
    std::string shaderCode = shaderSource.ReadShaderFromFile(vertSource);
    const GLchar *codeArray[] = { shaderCode.c_str() };

    glShaderSource(vertShader, 1, codeArray, NULL);

    glCompileShader(vertShader);

    GLint result;
    glGetShaderiv(vertShader, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE || shaderCode.length() < 1)
        return 0;

    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    if (fragShader == 0)
        return 0;

    std::string fragCode = shaderSource.ReadShaderFromFile(fragSource);
    const GLchar *fragArray[] = { fragCode.c_str() };

    glShaderSource(fragShader, 1, fragArray, NULL);

    glCompileShader(fragShader);

    GLint fragResult;
    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &fragResult);
    if (fragResult == GL_FALSE || fragCode.length() < 1)
        return 0;

    GLuint programHandle = glCreateProgram();
    if (programHandle == 0)
        return 0;

    glAttachShader(programHandle, vertShader);
    glAttachShader(programHandle, fragShader);

    glLinkProgram(programHandle);

    GLint linkStatus;
    glGetProgramiv(programHandle, GL_LINK_STATUS, &linkStatus);
    if (linkStatus == GL_FALSE)
        return 0;

    return programHandle;
}