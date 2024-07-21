#pragma once

class GLFWwindow;

class OutputKeyboardHandlers
{
public:
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
};