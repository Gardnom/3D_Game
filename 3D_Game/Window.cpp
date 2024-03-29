#include "Window.h"

void Window::ChangeWindow(WINDOW_SETTINGS newSettings) {
	GLFWwindow* newWin = CreateWindow(newSettings);
	glfwMakeContextCurrent(newWin);
	glfwDestroyWindow(m_Window);
	m_Window = newWin;
	ActivateCallbacks();
}

Window::Window(WINDOW_SETTINGS& settings) {
	m_Settings = settings;
	GLFWwindow* win = CreateWindow(m_Settings);
	m_Window = win;
	glfwMakeContextCurrent(m_Window);
}

Window::~Window() {
	glfwDestroyWindow(m_Window);
}

GLFWwindow* Window::CreateWindow(WINDOW_SETTINGS& settings) {
	GLFWmonitor* monitor = nullptr;
	if (settings.fullScreen) {
		monitor = glfwGetPrimaryMonitor();
	}

	GLFWwindow* window = glfwCreateWindow(
		settings.width,
		settings.height,
		settings.title,
		monitor,
		nullptr
	);

	return window;
}

void Window::ActivateCallbacks() {
	glfwSetKeyCallback(m_Window, m_KeyFun);
	glfwSetCursorPosCallback(m_Window, m_CursorPosFun);
	glfwSetScrollCallback(m_Window, m_ScrollFun);
}

void Window::SetCallbacks(GLFWkeyfun keyFun, GLFWcursorposfun cursorPosFun, GLFWscrollfun scrollFun) {
	m_KeyFun = keyFun;
	m_CursorPosFun = cursorPosFun;
	m_ScrollFun = scrollFun;
	ActivateCallbacks();
}
