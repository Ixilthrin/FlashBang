#include "CardDeckDispatchingMouseHandlers.h"

#include <GLFW/glfw3.h>

#include <iostream>

CardDeckEventTranslator *CardDeckDispatchingMouseHandlers::translator = 0;

void CardDeckDispatchingMouseHandlers::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (translator)
    {
        translator->mouseMoved(xpos, ypos);
    }
}

void CardDeckDispatchingMouseHandlers::cursor_enter_callback(GLFWwindow* window, int entered)
{
    if (entered)
    {
        //std::cout << "Hello Mouse" << std::endl;
    }
    else
    {
        //std::cout << "Goodbye Mouse" << std::endl;
    }
}
void CardDeckDispatchingMouseHandlers::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        if (translator)
        {
            translator->translateMouseEvent(CardDeckEventTranslator::EventType::LeftMouseDown);
        }
    }
    else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    {
        translator->translateMouseEvent(CardDeckEventTranslator::EventType::LeftMouseUp);
    }
    else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    {
        if (translator)
        {
            translator->translateMouseEvent(CardDeckEventTranslator::EventType::RightMouseDown);
        }
    }
    else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
    {
        translator->translateMouseEvent(CardDeckEventTranslator::EventType::RightMouseUp);
    }
}
void CardDeckDispatchingMouseHandlers::mouse_wheel_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    //std::cout << "Scrolling" << std::endl;
    translator->translateMouseWheelMove(yoffset);
}