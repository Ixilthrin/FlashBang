#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
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

using glm::vec4;

#include <GLFW/glfw3.h>

#include "BasicTriangle.h"
#include "BasicTriangleAlternateVBBinding.h"
#include "BasicWithoutLayoutQualifiers.h"
#include "TwoTrianglesUsingIndexBuffer.h"
#include "TriangleWithRotation.h"
#include "GenerateGeometryExample.h"
#include "BasicGraphicsLoop.h"
#include "BasicMouseSelection.h"
#include "BasicCardDeck.h"

#include "SampleModel.h"
#include "SampleProjectionModel.h"

extern "C"
{
    __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
    __declspec(dllexport) int NvOptimusEnablement = 0x00000001;
}

int showBasicTriangle()
{
    BasicTriangle drawer;
    return drawer.Start();
}

int showTriagleUsingAlternateVBBinding()
{
    BasicTriangleAlternateVBBinding drawer;
    return drawer.Start();
}

int showTriangleWithoutLocationQualifiers()
{
    BasicWithoutLayoutQualifiers drawer;
    return drawer.Start();
}

int showTwoTrianglesUsingIndexBuffer()
{
    TwoTrianglesUsingIndexBuffer drawer;
    return drawer.Start();
}

int showTriangleWithRotation()
{
    TriangleWithRotation drawer;
    return drawer.Start();
}

int showGenerateGeometryExample()
{
    GenerateGeometryExample drawer;
    return drawer.Start();
}

int runBasicGraphicsLoop()
{
    BasicGraphicsLoop program;
    SampleModel *sampleModel = new SampleModel();
    program.AddModel(sampleModel);
    return program.Run("Basic Graphics Loop");
}

int runBasicProjectionGraphicsLoop()
{
    BasicGraphicsLoop program;
    SampleProjectionModel *sampleModel = new SampleProjectionModel();
    program.AddModel(sampleModel);
    return program.Run("Basic Graphics Loop Using Projection Model");
}

int runBasicMouseSelection()
{
    BasicMouseSelection app;
    return app.Start();
}

int runBasicCardDeck()
{
    BasicCardDeck app;
    return app.Start();
}

void showVersionInfo(int result)
{
    cout << "Hello World from console" << endl;

    cout << "Result from setup = " << result << endl;

    const GLubyte *renderer = glGetString(GL_RENDERER);
    printf("Renderer: %s\n", renderer);

    const GLubyte *vender = glGetString(GL_VENDOR);
    printf("Vender: %s\n", vender);

    const GLubyte *version = glGetString(GL_VERSION);
    printf("Version: %s\n", version);

    const GLubyte *glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
    printf("Shading language version: %s", glslVersion);


    vec4 position = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

    glfwTerminate();

    int x;
    cin >> x;
}

int main(int argc, char* argv[])
{
    bool showInfo = false;
    int programNumber = 10;
    int result = 0;
    printf("Program Number %d\n", programNumber);
    switch (programNumber)
    {
    case 1:
        result = showBasicTriangle();
        break;
    case 2:
        result = showTriagleUsingAlternateVBBinding();
        break;
    case 3:
        result = showTriangleWithoutLocationQualifiers();
        break;
    case 4:
        result = showTwoTrianglesUsingIndexBuffer();
        break;
    case 5:
        result = showTriangleWithRotation();
        break;
    case 6:
        result = showGenerateGeometryExample();
        break;
    case 7:
        result = runBasicGraphicsLoop();
        break;
    case 8:
        result = runBasicProjectionGraphicsLoop();
        break;
    case 9:
        result = runBasicMouseSelection();
        break;
    case 10:
        result = runBasicCardDeck();
        break;
    }

    if (showInfo)
        showVersionInfo(result);

    return result;
}

