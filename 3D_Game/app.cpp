#include "app.h"
#include "Timer.h"

GLFWwindow* App::m_Window = nullptr;
std::shared_ptr<Window> App::m_PWindow = nullptr;

App::App(size_t windowWidth, size_t windowHeight, const char* windowTitle) {
	WINDOW_SETTINGS wSettings = { windowHeight, windowWidth, windowTitle, false };
	m_PWindow = std::make_shared<Window>(wSettings);

	m_PCamera = std::make_shared<Camera>();
	m_PRenderer = std::make_shared<Renderer>(m_PWindow, m_PCamera);

}

void App::SetupGui() {
	m_Gui = Gui(m_PRenderer);

	m_Gui.AddModule(m_PEntityCreatorGui);
	m_Gui.AddModule(m_PEntityListGui);
	m_Gui.AddModule(m_PLightSettingsGui);
}

void App::HandleCameraInput() {

	if (Input::KeyPressed(GLFW_KEY_O)) {
		m_CursorEnabled = !m_CursorEnabled;
		glfwSetInputMode(m_PWindow->m_Window, GLFW_CURSOR, m_CursorEnabled ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
	}

	if (Input::KeyHeld(GLFW_KEY_A)) {
		m_PCamera->m_Position -= glm::normalize(glm::cross(m_PCamera->GetDirection(), m_PCamera->m_Up)) * m_CameraSpeed;
	}

	if (Input::KeyHeld(GLFW_KEY_W)) {
		m_PCamera->m_Position += m_PCamera->GetDirection() * m_CameraSpeed;
	}

	if (Input::KeyHeld(GLFW_KEY_D)) {
		m_PCamera->m_Position += glm::normalize(glm::cross(m_PCamera->GetDirection(), m_PCamera->m_Up)) * m_CameraSpeed;
	}

	if (Input::KeyHeld(GLFW_KEY_S)) {
		m_PCamera->m_Position -= m_PCamera->GetDirection() * m_CameraSpeed;
	}


	const float mouseSensitivity = 0.1f;
	if (!m_CursorEnabled) {
		auto mousePos = Input::GetMousePosition();
		double offsetX = mousePos.x - mousePos.lastX;
		double offsetY = mousePos.y - mousePos.lastY;
		//printf("Offset Y: %f\n", offsetY);
		//printf("Offset X: %f\n", offsetX);

		m_PCamera->m_Yaw += (offsetX * mouseSensitivity);
		m_PCamera->m_Pitch -= (offsetY * mouseSensitivity);
		//camera->m_Yaw = glm::clamp(camera->m_Yaw, -179.0f, 0.0f);
		m_PCamera->m_Pitch = glm::clamp(m_PCamera->m_Pitch, -89.0f, 89.0f);
		printf("Pitch: %f\n", m_PCamera->m_Pitch);
		printf("Yaw: %f\n", m_PCamera->m_Yaw);

	}
}

GLFWwindowsizefun App::GetWindowSizeFun() {
	App* _app = this;

	auto fun = [](GLFWwindow* window, int width, int height) -> void{
		glViewport(0, 0, width, height);
		m_PWindow->m_Settings.width = width;
		m_PWindow->m_Settings.height = height;
		printf("Window width: %d\n", m_PWindow->m_Settings.width);

	};
	
	return static_cast<GLFWwindowsizefun>(fun);
}

App::App(GLFWwindow* window) {
	m_Window = window;
}

App::~App() {
	
}

Window& App::GetWindow() {
	return *m_PWindow;
}

void App::Update() {
	Input::Update();
	Timer::ClearTimings();
}

void App::Run() {

	// Initalize shader program
	m_PRenderer->SetupProgram();

	SetupGui();
	GameLoop();
}

void App::GameLoop() {
	ObjFileLoader objFileLoader;
	std::string objFilePath = "H:\\Blender\\Exports\\MultiColouredCube.obj";
	Mesh lightSourceMesh = objFileLoader.LoadMesh(objFilePath);
	 
	glm::vec4 lightColor = glm::vec4(235.0 / 255, 192.0 / 255, 52.0 / 255, 1.0f);

	for (auto& v : lightSourceMesh.m_Vertices)
		v.colour = lightColor;

	while (!glfwWindowShouldClose(m_PWindow->m_Window)) {

		Timer timer("Gameloop");
		glClearColor(153, 69, 86, 0xff);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_Gui.Begin();

		if (Input::KeyInState(GLFW_KEY_E, GLFW_PRESS)) {
			printf("E was pressed!\n");
			glfwSetWindowShouldClose(m_PWindow->m_Window, GLFW_TRUE);
		}

		if (Input::KeyInState(GLFW_KEY_E, GLFW_RELEASE)) {
			printf("E was released!\n");
		}
		if (Input::KeyPressed(GLFW_KEY_Y)) {
			//wSettings.fullScreen = !wSettings.fullScreen;
			//window->ChangeWindow(wSettings);
		}

		HandleCameraInput();


		m_PRenderer->BeginRender();

		m_PRenderer->SetLightPos(m_PLightSettingsGui->m_Pos);

		//m_PRenderer->UploadElementsInstanced(objMesh, std::vector<glm::vec3> {glm::vec3(0.0f, 0.0f, 0.0f)}, 2);
		//m_PRenderer->DrawInstanced();


		for (auto& pair : Engine::ECS::Ecs::m_Entities) {
			auto ent = pair.second;
			m_PRenderer->UploadElementsInstanced(const_cast<Mesh&>(ent->GetMesh()), OffVec{ glm::vec3(4.0f, 4.0f, 1.0f) }, 1);
			m_PRenderer->DrawInstanced();
		}

		m_PRenderer->UploadElementsInstanced(lightSourceMesh, OffVec{ m_PLightSettingsGui->m_Pos }, 1);
		m_PRenderer->DrawInstanced();

		m_Gui.Render();

		Update();

		glfwSwapBuffers(m_PWindow->m_Window);
		glfwPollEvents();
	}
}