#pragma once

#include "CardDeckEventTranslator.h"

#include <GLFW/glfw3.h>

class CardDeckDispatchingMouseHandlers
{
public:

    static CardDeckEventTranslator *translator;

    static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
    static void cursor_enter_callback(GLFWwindow* window, int entered);
    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    static void mouse_wheel_callback(GLFWwindow* window, double xoffset, double yoffset);
};