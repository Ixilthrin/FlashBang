#include "BasicWithoutLayoutQualifiers.h"

#include <iostream>
#include <glad/glad.h>
#include <stdio.h>
#include <sstream>
#include <fstream>

using std::endl;
using std::cout;
using std::cin;
using std::cerr;
using std::string;
using std::ifstream;
using std::stringstream;

#include <GLFW/glfw3.h>

#include "ShaderSource.h"
BasicWithoutLayoutQualifiers::BasicWithoutLayoutQualifiers()
{
}

BasicWithoutLayoutQualifiers::~BasicWithoutLayoutQualifiers()
{
}

int BasicWithoutLayoutQualifiers::Start()
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(640, 480, "Basic Triangle Without Location Qualifiers in Shaders", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -2;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGL())
        return -3;

    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    if (vertShader == 0)
        return -4;

    ShaderSource shaderSource;
    string shaderCode = shaderSource.ReadShaderFromFile("../FlashBang/shaders/nolocation.vert.glsl");
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

    string fragCode = shaderSource.ReadShaderFromFile("../FlashBang/shaders/nolocation.frag.glsl");
    const GLchar *fragArray[] = { fragCode.c_str() };

    glShaderSource(fragShader, 1, fragArray, NULL);

    glCompileShader(fragShader);

    GLint fragResult;
    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &fragResult);
    if (fragResult == GL_FALSE || fragCode.length() < 1)
        return -7;

    GLuint programHandle = glCreateProgram();
    if (programHandle == 0)
        return -8;

    glBindAttribLocation(programHandle, 0, "VertexPosition");
    glBindAttribLocation(programHandle, 1, "VertexColor");
    glBindFragDataLocation(programHandle, 0, "FragColor");

    glAttachShader(programHandle, vertShader);
    glAttachShader(programHandle, fragShader);

    glLinkProgram(programHandle);

    GLint linkStatus;
    glGetProgramiv(programHandle, GL_LINK_STATUS, &linkStatus);
    if (linkStatus == GL_FALSE)
        return -9;

    glUseProgram(programHandle);

    glDetachShader(programHandle, vertShader);
    glDetachShader(programHandle, fragShader);

    glDeleteShader(vertShader);
    glDeleteShader(fragShader);

    GLuint vaoHandle;

    float positionData[] =
    {
        -.8f, -.8f, 0.0f, 0.8f, -.8f, 0.0f, 0.0f, .8f, 0.0f
    };

    float colorData[] =
    {
        1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f
    };

    GLuint vboHandles[2];
    glGenBuffers(2, vboHandles);

    GLuint positionBufferHandle = vboHandles[0];
    GLuint colorBufferHandle = vboHandles[1];

    glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), positionData, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), colorData, GL_STATIC_DRAW);

    glGenVertexArrays(1, &vaoHandle);
    glBindVertexArray(vaoHandle);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindVertexBuffer(0, positionBufferHandle, 0, sizeof(GLfloat) * 3);
    glBindVertexBuffer(1, colorBufferHandle, 0, sizeof(GLfloat) * 3);

    glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexAttribBinding(0, 0);

    glVertexAttribFormat(1, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexAttribBinding(1, 1);

    glClearColor(0, 0, 0, 1);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(vaoHandle);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    return 0;
}
