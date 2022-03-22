#pragma once
#include <GLFW/glfw3.h>
#include "core.h"
#include "input.h"
#include "Window.h"
#include "Camera.h"
#include "Renderer.h"
#include "Gui.h"

class App {
public:
	App(GLFWwindow* window);
	App(size_t windowWidth, size_t windowHeight, const char* windowTitle);
	
	~App();
	
	void Update();
	Window& GetWindow();

	void Run();
	
	GLFWwindowsizefun GetWindowSizeFun();

private: 
	static GLFWwindow* m_Window;
	static std::shared_ptr<Window> m_PWindow;
	std::shared_ptr<Camera> m_PCamera;
	std::shared_ptr<Renderer> m_PRenderer;

	float m_CameraSpeed = 0.1f;
	bool m_CursorEnabled = true;

	Gui m_Gui;

	// Gui modules
	std::shared_ptr<EntityCreatorGui> m_PEntityCreatorGui = std::make_shared<EntityCreatorGui>();
	std::shared_ptr<EntityListGui> m_PEntityListGui = std::make_shared<EntityListGui>();
	std::shared_ptr<LightSettingsGui> m_PLightSettingsGui = std::make_shared<LightSettingsGui>();

private:
	
	void GameLoop();

	void SetupGui();
	void HandleCameraInput();

	friend class Input;
};