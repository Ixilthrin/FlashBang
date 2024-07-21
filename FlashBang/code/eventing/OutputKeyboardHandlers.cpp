#include "OutputKeyboardHandlers.h"

#include <GLFW/glfw3.h>

#include <iostream>

void OutputKeyboardHandlers::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_E && action == GLFW_PRESS)
    {
        std::cout << "E" << std::endl;
    }
}
