#pragma once
#include "core.h"
#include <GLFW/glfw3.h>


typedef struct WINDOW_SETTINGS {
	int height, width;
	const char* title;
	bool fullScreen;
} WINDOW_SETTINGS;

class Window {
public:
	Window(WINDOW_SETTINGS& settings);
	~Window();

	GLFWwindow* CreateWindow(WINDOW_SETTINGS& settings);
	void ChangeWindow(WINDOW_SETTINGS newSettings);
	void SetCallbacks(GLFWkeyfun keyFun, GLFWcursorposfun cursorPosFun, GLFWscrollfun scrollFun, GLFWwindowsizefun windowSizeFun);

	GLFWwindow* m_Window;

	const int Height() {
		return m_Settings.height;
	}

	const int Width() {
		return m_Settings.width;
	}

	friend class App;

private:
	void ActivateCallbacks();
	GLFWkeyfun m_KeyFun;
	GLFWcursorposfun m_CursorPosFun;
	GLFWscrollfun m_ScrollFun;
	GLFWwindowsizefun m_WindowSizeFun;
	WINDOW_SETTINGS m_Settings;
};