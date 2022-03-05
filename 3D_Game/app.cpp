#include "app.h"
#include "Timer.h"

GLFWwindow* App::m_Window = nullptr;

void App::Update() {
	Input::Update();
	Timer::ClearTimings();
}

App::App(GLFWwindow* window) {
	m_Window = window;
}