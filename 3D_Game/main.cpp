#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include "core.h"
#include "input.h"
#include "app.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "File.h"
#include "Timer.h"
#include "Window.h"
#include "Renderer.h"
#include "Gui.h"
#include "Constants.h"
#include "Shapes.h"

#include "ObjFileLoader.h"
#include <algorithm>

#include "Errors.h"


void CreateAndUploadVertexData(ShaderProgram& program, WINDOW_SETTINGS& window) {
	glm::vec4 colour(0.0f, 0.0f, 0.0f, 1.0f);
	glm::vec4 colour2(153.0f / 255.0f, 69.0f / 255.0f, 86.0f / 255.0f, 1.0f);
	

	/*Vertex vertArray[3] = {
		{glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)},
		{glm::vec3(0.0f, 0.5f, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)},
		{glm::vec3(0.5f, -0.5f, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)},
	};*/

	/*Vertex vertArray[6] = {
		{glm::vec3(0.5f, 0.5f, 0.0f), colour},
		{glm::vec3(-0.5f, -0.5f, 0.0f), colour},
		{glm::vec3(-0.5f, 0.5f, 0.0f), colour},
		
		{glm::vec3(0.5f, 0.5f, 0.0f), colour2},
		{glm::vec3(0.5f, -0.5f, 0.0f), colour2},
		{glm::vec3(-0.5f, -0.5f, 0.0f), colour2}
	};*/

	/*Vertex vertArray[4] = {
		{glm::vec3(0.5f, 0.5f, 0.0f), colour},
		{glm::vec3(0.5f, -0.5f, 0.0f), colour},
		{glm::vec3(-0.5f, -0.5f, 0.0f), colour},
		{glm::vec3(-0.5f, 0.5f, 0.0f), colour},
	};

	uint32_t indicies[6] = {
		0, 2, 3,
		0, 1, 2
	};*/

	Vertex vertArray[8] = {
		// Top
		{glm::vec3(0.0f, 1.0f, 1.0f), colour},
		{glm::vec3(1.0f, 1.0f, 1.0f), colour},
		{glm::vec3(1.0f, 0.0f, 1.0f), colour},
		{glm::vec3(0.0f, 0.0f, 0.1f), colour},

		// Front
		{glm::vec3(0.0, 1.0f, 0.0f), colour},
		{glm::vec3(0.0, 0.0f, 0.0f), colour},
		{glm::vec3(1.0, 0.0f, 0.0f), colour},
		
		{glm::vec3(1.0, 1.0f, 0.0f), colour},

	};

	uint32_t indicies[6 * 3] = {
		// Face
		3, 2, 5,
		2, 6, 5,
		
		// Top
		0, 1, 3,
		1, 2, 3,

		// Left
		0, 3, 4,
		3, 5, 4,
	};

	// Transformation matrix
	glm::vec3 scale(2.0f, 2.0f, 2.0f);
	float rotation = 0.0f;
	glm::vec3 position(0.0f, 0.0f, 0.0f);

	glm::mat4 transform = glm::scale(scale);
	transform = glm::rotate(transform, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
	transform = glm::translate(transform, position);

	// View matrix
	glm::vec3 eye = glm::vec3(5.0f, 10.0f, 20.0f);
	glm::vec3 center = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 viewMatrix = glm::lookAt(eye, center, up);
	
	// Projection matrix
	float fov = 45.0f;
	float _near = 0.01f;
	float _far = 100.0f;
	float windowAspect = ((float)window.width / (float)window.height);
	glm::mat4 projection = glm::perspective(fov, windowAspect, _near, _far);


	// Create and bind Vertex array object
	GLuint vaoId;
	glCreateVertexArrays(1, &vaoId);
	glBindVertexArray(vaoId);
	
	// Create and bind element buffer array
	GLuint eboId;
	glGenBuffers(1, &eboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboId);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		sizeof(indicies),
		indicies,
		GL_STATIC_DRAW
	);

	// Create and bind buffer
	GLuint vboId;
	glCreateBuffers(1, &vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertArray), vertArray, GL_STATIC_DRAW);

	// Set attribute position
	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);
	
	// Set attribute colour
	glVertexAttribPointer(1, 4, GL_FLOAT, false, sizeof(Vertex), (void*)offsetof(Vertex, colour));
	glEnableVertexAttribArray(1);

	/*GLuint transMatId = glGetUniformLocation(program.m_ProgramId, "uTransform");
	GLuint viewMatId = glGetUniformLocation(program.m_ProgramId, "uView");
	GLuint projMatId = glGetUniformLocation(program.m_ProgramId, "uProjection");

	glUniformMatrix4fv(transMatId, 1, GL_FALSE, glm::value_ptr(transform));
	glUniformMatrix4fv(viewMatId, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniformMatrix4fv(projMatId, 1, GL_FALSE, glm::value_ptr(projection));*/
	program.Bind();
	
	Timer timer("UploadUniforms");
	program.UploadUniformMatrix4fv("uTransform", transform);
	program.UploadUniformMatrix4fv("uView", viewMatrix);
	program.UploadUniformMatrix4fv("uProjection", projection);

	//glDrawArrays(GL_TRIANGLES, 0, 2 * 3);
	glDrawElements(GL_TRIANGLES, 6 * 3, GL_UNSIGNED_INT, 0);
}

typedef std::vector<glm::vec3> OffVec;

int main() {
	

	if (!glfwInit()) {
		printf("Failed to initialize GLFW!\n");
		DisplayErrorCritical("Failed to initialize GLFW!");
		return -1;
	}

	

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	uint32_t windowWidth = 1280;
	uint32_t windowHeight = 720;
	const char* windowTitle = "3D Game!";


	WINDOW_SETTINGS wSettings = { windowHeight, windowWidth, windowTitle, false };

	std::shared_ptr<Window> window =  std::make_shared<Window>(wSettings);

	if (window->m_Window == nullptr) {
		printf("Failed to create window\n");
		DisplayErrorCritical("Failed to create window\n");
		glfwTerminate();
		return -1;
	}

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("Failed to initialize Glad.\n");
		DisplayErrorCritical("Failed to initialize Glad.\n");
		return -1;
	}

	glViewport(0, 0, windowWidth, windowHeight);
	
	App app(window->m_Window);
	window->SetCallbacks(Key_Callback, cursor_position_callback, scroll_callback);
	
	
	
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsClassic();
	ImGui_ImplGlfw_InitForOpenGL(window->m_Window, true);
	ImGui_ImplOpenGL3_Init("#version 460");
	
	//Renderer ren(window);
	auto camera = std::make_shared<Camera>();
	std::shared_ptr<Renderer> ren = std::make_shared<Renderer>(window, camera);
	ren->SetupProgram();
	
	Gui gui(ren);
	

	float ObjectScale = 1;
	float xPos = 0.0f;
	float yPos = 0.0f;
	float zPos = 0.0f;

	Cube cube(glm::vec3(1.0f, 1.0f, 1.0f));
	Cube cube2(glm::vec3(2.0f, 2.0f, 2.0f));
	//std::shared_ptr<std::vector<Cube>> cubes = std::make_shared<std::vector<Cube>>();
	std::vector<std::shared_ptr<Cube>> cubes = { std::make_shared<Cube>(cube), std::make_shared<Cube>(cube2) };

	Entity cubeEntity(cube.m_Mesh, glm::vec3(5.0f, 5.0f, 5.0f));
	Entity cubeEntity2(cube.m_Mesh, glm::vec3(6.0f, 5.0f, 5.0f));
	std::vector<std::shared_ptr<Entity>> cubeEntities = { std::make_shared<Entity>(cubeEntity), std::make_shared<Entity>(cubeEntity2) };
	//cubes->push_back(cube);
	//cubes->push_back(cube2);

	std::shared_ptr<CubeGui> cubeGui = std::make_shared<CubeGui>();
	cubeGui->m_Cubes = &cubes;
	gui.AddModule(cubeGui);

	float CameraSpeed = 0.1f;

	bool cursorEnabled = false;

	ObjFileLoader objFileLoader;
	std::string objFilePath = "H:\\Blender\\Exports\\MultiColouredCube.obj";
	Mesh objMesh = objFileLoader.LoadMesh(objFilePath);


	// To fix indcies starting at index 1 in obj file
	/*std::vector<int> _indicies;
	_indicies.reserve(objMesh.m_Indicies.size());
	std::transform(objMesh.m_Indicies.begin(), objMesh.m_Indicies.end(),
		std::back_inserter(_indicies), [](int index) {return index - 1; });

	objMesh.m_Indicies = _indicies;*/
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);

	std::shared_ptr<EntityCreatorGui> ecg = std::make_shared<EntityCreatorGui>();
	std::shared_ptr<EntityListGui> entityListGui = std::make_shared<EntityListGui>();
	gui.AddModule(ecg);
	gui.AddModule(entityListGui);
	
	while (!glfwWindowShouldClose(window->m_Window)) {
		
		Timer timer("Gameloop");
		glClearColor(153, 69, 86, 0xff);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		gui.Begin();

		if (Input::KeyInState(GLFW_KEY_E, GLFW_PRESS)) {
			printf("E was pressed!\n");
			glfwSetWindowShouldClose(window->m_Window, GLFW_TRUE);
		}

		if (Input::KeyInState(GLFW_KEY_E, GLFW_RELEASE)) {
			printf("E was released!\n");
		}
		if (Input::KeyPressed(GLFW_KEY_Y)) {
			wSettings.fullScreen = !wSettings.fullScreen;
			window->ChangeWindow(wSettings);
		}

		if (Input::KeyHeld(GLFW_KEY_2)) {
			ObjectScale += 0.1f;
		}

		if (Input::KeyHeld(GLFW_KEY_1)) {
			ObjectScale -= 0.1f;
		}

		if (Input::KeyPressed(GLFW_KEY_UP)) {
			yPos += 0.1f;
		}

		if (Input::KeyPressed(GLFW_KEY_DOWN)) {
			yPos -= 0.1f;
		}

		if (Input::KeyPressed(GLFW_KEY_RIGHT)) {
			xPos += 0.1f;
		}

		if (Input::KeyPressed(GLFW_KEY_LEFT)) {
			xPos -= 0.1f;
		}

		if (Input::KeyHeld(GLFW_KEY_A)) {
			camera->m_Position -= glm::normalize(glm::cross(camera->GetDirection(), camera->m_Up)) * CameraSpeed;
		}

		if (Input::KeyHeld(GLFW_KEY_W)) {
			camera->m_Position += camera->GetDirection() * CameraSpeed;
		}

		if (Input::KeyHeld(GLFW_KEY_D)) {
			camera->m_Position += glm::normalize(glm::cross(camera->GetDirection(), camera->m_Up)) * CameraSpeed;
		}

		if (Input::KeyHeld(GLFW_KEY_S)) {
			camera->m_Position -= camera->GetDirection() * CameraSpeed;
		}

		if (Input::KeyPressed(GLFW_KEY_O)) {
			cursorEnabled = !cursorEnabled;
			glfwSetInputMode(window->m_Window, GLFW_CURSOR, cursorEnabled ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
		}

		const float mouseSensitivity = 0.1f;
		if (cursorEnabled) {
			auto mousePos = Input::GetMousePosition();
			double offsetX = mousePos.x - mousePos.lastX;
			double offsetY = mousePos.y - mousePos.lastY;
			//printf("Offset Y: %f\n", offsetY);
			//printf("Offset X: %f\n", offsetX);

			camera->m_Yaw += (offsetX * mouseSensitivity);
			camera->m_Pitch -= (offsetY * mouseSensitivity);
			//camera->m_Yaw = glm::clamp(camera->m_Yaw, -179.0f, 0.0f);
			camera->m_Pitch = glm::clamp(camera->m_Pitch, -89.0f, 89.0f);
			printf("Pitch: %f\n", camera->m_Pitch);
			printf("Yaw: %f\n", camera->m_Yaw);

		}

		glm::vec4 colour(0.0f, 0.0f, 0.0f, 1.0f);

		//auto cubeRenderInfo = cube.GetRenderInfo();

		ren->BeginRender();
		

		ren->UploadElementsInstanced(objMesh, std::vector<glm::vec3> {glm::vec3(0.0f, 0.0f, 0.0f)}, 2);
		ren->DrawInstanced();

		ren->UploadElementsInstanced(cubeEntities);
		ren->DrawInstanced();
		
	
		for (auto& pair : Engine::ECS::Ecs::m_Entities) {
			auto ent = pair.second;
			ren->UploadElementsInstanced(const_cast<Mesh&>(ent->GetMesh()), OffVec{ glm::vec3(4.0f, 4.0f, 1.0f) }, 1);
			ren->DrawInstanced();
		}

		//ren->UploadElements(cubeRenderInfo.first, cubeRenderInfo.second);
		//ren->Draw();
		
		gui.Render();
		
		app.Update();
		//CreateAndUploadVertexData(program, wSettings);

		glfwSwapBuffers(window->m_Window);
		glfwPollEvents();
		
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();

}