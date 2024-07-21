#include "GenerateGeometryExample.h"

#include <chrono>
#include <iostream>
#include <glad/glad.h>
#include <stdio.h>
#include <sstream>
#include <fstream>
#include <vector>

using std::endl;
using std::cout;
using std::cin;
using std::cerr;
using std::string;
using std::ifstream;
using std::stringstream;
using std::chrono::system_clock;
using std::chrono::duration;
using std::vector;

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ShaderSource.h"

GenerateGeometryExample::GenerateGeometryExample()
{
}


GenerateGeometryExample::~GenerateGeometryExample()
{
}

int GenerateGeometryExample::Start()
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_SAMPLES, 4); 
    window = glfwCreateWindow(800, 600, "Rotating Triangles", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -2;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);

    if (!gladLoadGL())
        return -3;

    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    if (vertShader == 0)
        return -4;

    ShaderSource shaderSource;
    string shaderCode = shaderSource.ReadShaderFromFile("../FlashBang/shaders/rotation.vert.glsl");
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

    GLuint programHandle = glCreateProgram();
    if (programHandle == 0)
        return -8;

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

    float x = -0.6f;
    float y = -0.6f;
    float z = 0.0f;

    vector<float> positionData =
    {
        x, y, z
    };

    vector<float> colorData =
    {
        .82f, 0.41f, 0.14f
    };

    vector<int> indexData;

    int index = 0;
    float previousY = y;
    y -= .05f;
    positionData.push_back(x);
    positionData.push_back(y);
    positionData.push_back(z);
    colorData.push_back(.82f);
    colorData.push_back(.41f);
    colorData.push_back(.14f);

    for (int c = 0; c < 10; ++c)
    {
        y = previousY + .01 * c;
        x += .05f;
        if (c < 5)
            z -= .05f;
        else
            z += .05f;
        positionData.push_back(x);
        positionData.push_back(y);
        positionData.push_back(z);
        colorData.push_back(1.0f);
        colorData.push_back(0.0f);
        colorData.push_back(0.0f);
        indexData.push_back(index);
        indexData.push_back(index + 1);
        indexData.push_back(index + 2);
        index += 1;
        previousY = y;
        y -= .05f;
        positionData.push_back(x);
        positionData.push_back(y);
        positionData.push_back(z);
        colorData.push_back(.82f);
        colorData.push_back(.41f);
        colorData.push_back(.14f);
        indexData.push_back(index);
        indexData.push_back(index + 2);
        indexData.push_back(index + 1);
        index += 1;
    }


    GLuint vboHandles[2];
    glGenBuffers(2, vboHandles);

    GLuint positionBufferHandle = vboHandles[0];
    GLuint colorBufferHandle = vboHandles[1];

    glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
    glBufferData(GL_ARRAY_BUFFER, positionData.size() * sizeof(float), &positionData[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
    glBufferData(GL_ARRAY_BUFFER, positionData.size() * sizeof(float), &colorData[0], GL_STATIC_DRAW);

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

    glEnable(GL_MULTISAMPLE);

    float angle = 0.0f;
    float rotationRate = 2.0f * 3.14159f / 10.0f;

    int frameCount = 0;
    auto startTime = system_clock::now();
    auto currentTime = system_clock::now();
    duration<double> deltaTime;

    while (!glfwWindowShouldClose(window))
    {
        frameCount++;

        //if (frameCount % 80 == 0)
            glClear(GL_COLOR_BUFFER_BIT);

        currentTime = system_clock::now();
        deltaTime = currentTime - startTime;
        angle = rotationRate * (float) deltaTime.count();

        glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 0.0f, 1.0f));

        GLuint location = glGetUniformLocation(programHandle, "RotationMatrix");
        if (location >= 0)
            glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(rotationMatrix));

        glBindVertexArray(vaoHandle);

        glDrawElements(GL_TRIANGLES, indexData.size(), GL_UNSIGNED_INT, &indexData[0]);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    duration<double> totalTime = system_clock::now() - startTime;
    cout << "Total time: " << totalTime.count() << endl;
    cout << "Total frames: " << frameCount << endl;
    cout << "Frames/second: " << (double)frameCount / totalTime.count() << endl;
    return 0;
}