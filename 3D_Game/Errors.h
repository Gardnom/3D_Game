#pragma once
#include <string>
#include "Window.h"

#ifdef _WIN32
#define GLFW_EXPOSE_NATIVE_WIN32
#endif

#include <GLFW/glfw3native.h>

inline void DisplayErrorCritical(std::string message, std::string title = "Error");
inline void DisplayErrorCriticalWnd(Window& wnd, std::string message, std::string title = "Error");


#ifdef _WIN32
#include <WinUser.h>
void DisplayErrorCriticalWnd(Window& wnd, std::string message, std::string title) {
	HWND hWnd = glfwGetWin32Window(wnd.m_Window);
	UINT type = MB_ICONERROR;
	MessageBox(hWnd, message.c_str(), title.c_str(), type);
}

void DisplayErrorCritical(std::string message, std::string title) {
	UINT type = MB_ICONERROR;
	MessageBox(NULL, message.c_str(), title.c_str(), type);
}
#else
void DisplayErrorCriticalWnd(Window& wnd, std::string message, std::string title = "Error") {
	printf(message.c_str());
}
void DisplayErrorCritical(std::string message, std::string title = "Error") {
	printf(message.c_str());
}

#endif