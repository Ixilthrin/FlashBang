#include "BasicGraphicsLoop.h"

#include <chrono>
#include <iostream>
#include <glad/glad.h>
#include <stdio.h>

using std::endl;
using std::cout;
using std::cin;
using std::cerr;
using std::chrono::system_clock;
using std::chrono::duration;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

BasicGraphicsLoop::BasicGraphicsLoop()
{
}

BasicGraphicsLoop::~BasicGraphicsLoop()
{
}

void BasicGraphicsLoop::AddModel(IModel *model)
{
    _models.push_back(model);
}

int BasicGraphicsLoop::Run(string title)
{
    int createWindowResult = _window.CreateWindow(title);
    if (createWindowResult != 0)
        return createWindowResult;

    if (!gladLoadGL())
        return -3;

    glClearColor(0, 0, 0, 1);

    glEnable(GL_MULTISAMPLE);

    for (auto model : _models)
        model->Setup();

    int frameCount = 0;
    auto startTime = system_clock::now();

    while (_window.IsOpen())
    {
        frameCount++;

        glClear(GL_COLOR_BUFFER_BIT);

        for (auto model : _models)
            model->Start();

        _window.SwapBuffers();

        _window.PollEvents();
    }

    duration<double> totalTime = system_clock::now() - startTime;
    cout << "Total time: " << totalTime.count() << endl;
    cout << "Total frames: " << frameCount << endl;
    cout << "Frames/second: " << (double)frameCount / totalTime.count() << endl;

    for (auto model : _models)
        delete model;

    return 0;
}
