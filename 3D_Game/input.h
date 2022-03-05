#pragma once
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <vector>
#include <unordered_map>
#include <map>
#include "app.h"

typedef struct MousePosition {
	double x, y;
	double scrollX, scrollY;
	double lastX, lastY;

} MousePosition;

class Input {
	
public:
	static bool KeyPressed(int key);
	//static bool KeyReleased(int key);
	static bool KeyHeld(int key);
	static bool KeyInState(int key, int state);

	friend void Key_Callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	friend void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
	friend class App;

public:
	static MousePosition mousePosition;
	static const MousePosition GetMousePosition();

private:
	static void Update();
	// Key -> action
	static std::unordered_map<int, std::vector<int>> m_KeysPressed;
};

inline void Key_Callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

	// Check if key exists
	auto it = Input::m_KeysPressed.find(key);
	if (it == Input::m_KeysPressed.end()) {
		// Create new vec there if key does not exist
		Input::m_KeysPressed.emplace(key, std::vector<int>({action}));
	}else
		Input::m_KeysPressed[key].emplace_back(action);
}

inline void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	
	Input::mousePosition.x = xpos;
	Input::mousePosition.y = ypos;
}

inline void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	Input::mousePosition.scrollX = xoffset;
	Input::mousePosition.scrollY = yoffset;
}