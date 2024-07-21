#pragma once

class GLFWwindow;

class Basic3DEngine
{
private:
	int _windowWidth;
	int _windowHeight;
	GLFWwindow* _window;
	int initializeWindow();
	int initializeOpenGL();
public:
};