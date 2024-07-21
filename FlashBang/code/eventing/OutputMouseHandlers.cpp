#include "OutputMouseHandlers.h"

#include <GLFW/glfw3.h>

#include <iostream>

void OutputMouseHandlers::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    std::cout << "x: " << xpos << "   y: " << ypos << std::endl;
}

void OutputMouseHandlers::cursor_enter_callback(GLFWwindow* window, int entered)
{
    if (entered)
    {
        std::cout << "Hello Mouse" << std::endl;
    }
    else
    {
        std::cout << "Goodbye Mouse" << std::endl;
    }
}
void OutputMouseHandlers::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        std::cout << "Left was pressed" << std::endl;
    }
}
void OutputMouseHandlers::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    std::cout << "Scrolling" << std::endl;
}