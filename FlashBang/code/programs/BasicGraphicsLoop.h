#pragma once

#include <string>
#include <vector>
using std::string;
using std::vector;

#include "IModel.h"
#include "GLFWGraphicsWindow.h"

class BasicGraphicsLoop
{
public:
    BasicGraphicsLoop();
    virtual ~BasicGraphicsLoop();
    void AddModel(IModel *model);
    int Run(string title);

private:
    vector<IModel*> _models;
    GLFWGraphicsWindow _window;
};

