#include "input.h"
#include "app.h"

std::unordered_map<int, std::vector<int>> Input::m_KeysPressed;

MousePosition Input::mousePosition;

bool Input::KeyPressed(int key) {
	return KeyInState(key, GLFW_PRESS);
}

bool Input::KeyInState(int key, int state) {
	auto it = m_KeysPressed.find(key);
	if (it == m_KeysPressed.end()) return false;

	for (auto& _state : m_KeysPressed[key]) {
		if (state == _state) return true;
	}

	return false;
}


void Input::Update() {
	m_KeysPressed.clear();

	mousePosition.lastX = mousePosition.x;
	mousePosition.lastY = mousePosition.y;
}

const MousePosition Input::GetMousePosition() {
	return mousePosition;
}

bool Input::KeyHeld(int key) {
	return glfwGetKey(App::m_PWindow->m_Window, key) != GLFW_RELEASE;
}

