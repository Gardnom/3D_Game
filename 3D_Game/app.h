#pragma once
#include <GLFW/glfw3.h>
#include "input.h"

class App {
public:
	App(GLFWwindow* window);
	void Update();
private: 
	static GLFWwindow* m_Window;

	friend class Input;
};