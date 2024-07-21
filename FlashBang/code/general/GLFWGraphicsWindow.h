#pragma once

struct GLFWwindow;

#include <string>

using std::string;

class GLFWGraphicsWindow
{
private:
    GLFWwindow *_window;
public:
    GLFWGraphicsWindow();
    ~GLFWGraphicsWindow();
    int CreateWindow(string title);
    bool IsOpen();
    void SwapBuffers();
    void PollEvents();
};

