#include "GLFWGraphicsWindow.h"

#include <GLFW/glfw3.h>

GLFWGraphicsWindow::GLFWGraphicsWindow()
{
    _window = nullptr;
}

GLFWGraphicsWindow::~GLFWGraphicsWindow()
{
}

int GLFWGraphicsWindow::CreateWindow(string title)
{
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_SAMPLES, 4);
    _window = glfwCreateWindow(800, 600, title.c_str(), NULL, NULL);
    if (!_window)
    {
        glfwTerminate();
        return -2;
    }

    glfwMakeContextCurrent(_window);
    glfwSwapInterval(0);

    return 0;
}

bool GLFWGraphicsWindow::IsOpen()
{
    return !glfwWindowShouldClose(_window);
}

void GLFWGraphicsWindow::SwapBuffers()
{
    glfwSwapBuffers(_window);
}

void GLFWGraphicsWindow::PollEvents()
{
    glfwPollEvents();
}