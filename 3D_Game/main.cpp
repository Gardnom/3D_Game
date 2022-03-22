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



ImGuiIO& SetupImGui(const Window& wnd) {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	ImGui::StyleColorsClassic();
	ImGui_ImplGlfw_InitForOpenGL(wnd.m_Window, true);
	ImGui_ImplOpenGL3_Init("#version 460");
	return io;
	
}



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

	
	App app(windowWidth, windowHeight, windowTitle);
	

	if (app.GetWindow().m_Window == nullptr) {
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
	
	app.GetWindow().SetCallbacks(Key_Callback, cursor_position_callback, scroll_callback, app.GetWindowSizeFun());
	
	SetupImGui(app.GetWindow());
	
	
	ObjFileLoader objFileLoader;
	std::string objFilePath = "H:\\Blender\\Exports\\MultiColouredCube.obj";
	Mesh objMesh = objFileLoader.LoadMesh(objFilePath);


	// To fix indcies starting at index 1 in obj file
	/*std::vector<int> _indicies;
	_indicies.reserve(objMesh.m_Indicies.size());
	std::transform(objMesh.m_Indicies.begin(), objMesh.m_Indicies.end(),
		std::back_inserter(_indicies), [](int index) {return index - 1; });

	objMesh.m_Indicies = _indicies;*/
	
	// Gl configurations
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);

	// Start app which will start gameloop
	app.Run();

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();

}