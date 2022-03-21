/*#include "Errors.h"

#ifdef _WIN32
#define GLFW_EXPOSE_NATIVE_WIN32
#endif

#include <GLFW/glfw3native.h>

#ifdef _WIN32
#include <WinUser.h>
void DisplayErrorCriticalWnd(Window& wnd, std::string message, std::string title) {
	HWND hWnd = glfwGetWin32Window(wnd.m_Window);
	UINT type;
	type | MB_ICONERROR;
	MessageBox(hWnd, message.c_str(), title.c_str(), type);
}

void DisplayErrorCritical(std::string message, std::string title) {
	UINT type;
	type | MB_ICONERROR;
	MessageBox(NULL, message.c_str(), title.c_str(), type);
}
#else
void DisplayErrorCriticalWnd(Window& wnd, std::string message, std::string title) {
	printf(message.c_str());
}
void DisplayErrorCritical(std::string message, std::string title) {
	printf(message.c_str());
}

#endif
*/