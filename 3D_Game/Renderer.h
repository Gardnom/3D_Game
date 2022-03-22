#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <memory>
#include "core.h"
#include "ShaderProgram.h"
#include "Window.h"
#include "Mesh.h"
#include "Camera.h"
#include "Entity.h"

class RendererOptionsGui;

class Renderer {

public:
	Renderer(std::shared_ptr<Window>& window, std::shared_ptr<Camera>& cam);
	~Renderer();

	void BeginRender();
	//void UploadVertices(std::vector<Vertex>& vertices);
	//void UploadIndicies(std::vector<int>& indicies);

	void UploadElements(std::vector<Vertex>& vertices, std::vector<int>& indicies);
	void UploadElementsInstanced(Mesh& mesh, std::vector<glm::vec3> offsets, uint32_t numInstances);
	void UploadElementsInstanced(std::vector<std::shared_ptr<Entity>>& entities);

	void SetTransform(glm::mat4 transformMatrix);
	void Transform(glm::vec3 scale, float rotation, glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f));

	void SetLightPos(glm::vec3& pos);

	void SetVertices();
	void SetIndicies();
	void Draw();

	void DrawInstanced();

	void SetupProgram();

	glm::vec3 m_Eye = glm::vec3(0.0f, 0.0f, 20.0f);
	
	std::shared_ptr<RendererOptionsGui> m_Gui;
		
private:
	glm::vec3 m_GlobalScale;
	glm::vec3 m_Up = glm::vec3(0.0f, 1.0f, 0.0f);
	const glm::vec3 m_Center = glm::vec3(0.0f, 0.0f, 0.0f);

	std::shared_ptr<Camera> m_Camera;

	float m_Fov = 45.0f;
	float m_Near = 0.01f;
	float m_Far = 100.0f;
	float m_WindowAspect;

	size_t m_NumTris;
	size_t m_IndiciesCount;
	size_t m_InstanceCount;


	std::unique_ptr<ShaderProgram> m_Program;
	std::shared_ptr<Window> m_Window;
	
	
};