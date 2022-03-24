#include "Renderer.h"
#include "File.h"
#include "Timer.h"
#include <algorithm>

void Renderer::BeginRender() {
	glm::vec3 scale(1.0f, 1.0f, 1.0f);
	float rotation = 0.0f;
	glm::vec3 position(0.0f, 0.0f, 0.0f);

	glm::mat4 transform = glm::scale(scale);
	transform = glm::rotate(transform, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
	transform = glm::translate(transform, position);

	// View matrix
	//glm::mat4 viewMatrix = glm::lookAt(m_Eye, m_Center, m_Up);
	glm::mat4 viewMatrix = m_Camera->GetViewMatrix();

	// Projection matrix
	float windowAspect = ((float)m_Window->Width() / (float)m_Window->Height());
	glm::mat4 projection = glm::perspective(m_Gui->m_Fov, windowAspect, m_Near, m_Far);

	m_Program->Bind();
	Timer timer("Upload uniforms");
	m_Program->UploadUniformMatrix4fv("uTransform", transform);
	m_Program->UploadUniformMatrix4fv("uView", viewMatrix);
	m_Program->UploadUniformMatrix4fv("uProjection", projection);
	
	//m_NumTris = 0;
	//m_IndiciesCount = 0;
}


void Renderer::SetTransform(glm::mat4 transformMatrix) {
	m_Program->UploadUniformMatrix4fv("uTransform", transformMatrix);
}

void Renderer::SetLightPos(glm::vec3& pos) {
	m_Program->UploadUniformVec3fv("uLightPos", pos);

	glm::vec3 lightColor(235.0 / 255, 192.0 / 255, 52.0 / 255);
	m_Program->UploadUniformVec3fv("uLightColor", lightColor);
}

void Renderer::Transform(glm::vec3 scale, float rotation, glm::vec3 position) {

	glm::mat4 transform = glm::scale(scale);
	transform = glm::rotate(transform, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
	transform = glm::translate(transform, position);
	SetTransform(transform);
}

void Renderer::UploadElements(std::vector<Vertex>& vertices, std::vector<int>& indicies) {
	Timer timer("Renderer::UploadElements");
	GLuint vaoId;
	glCreateVertexArrays(1, &vaoId);
	glBindVertexArray(vaoId);

	// Create and bind element buffer array
	GLuint eboId;
	glGenBuffers(1, &eboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboId);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		sizeof(int) * indicies.size(),
		indicies.data(),
		GL_STATIC_DRAW
	);

	// Create and bind buffer
	GLuint vboId;
	glCreateBuffers(1, &vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	// Set attribute position
	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);

	// Set attribute colour
	glVertexAttribPointer(1, 4, GL_FLOAT, false, sizeof(Vertex), (void*)offsetof(Vertex, colour));
	glEnableVertexAttribArray(1);

	m_NumTris = indicies.size() / 3;
	m_IndiciesCount = indicies.size();
}


void Renderer::UploadElementsInstanced(Mesh& mesh, std::vector<glm::vec3> offsets, uint32_t numInstances) {
	Timer timer("Renderer::UploadElementsInstanced");
	GLuint vaoId;
	glCreateVertexArrays(1, &vaoId);
	glBindVertexArray(vaoId);

	// Create and bind element buffer array
	GLuint eboId;
	glGenBuffers(1, &eboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboId);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		sizeof(int) * mesh.m_Indicies.size(),
		mesh.m_Indicies.data(),
		GL_STATIC_DRAW
	);

	// Create and bind buffer
	GLuint vboId;
	glCreateBuffers(1, &vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, mesh.m_Vertices.size() * sizeof(Vertex), mesh.m_Vertices.data(), GL_STATIC_DRAW);

	// Set attribute position
	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);

	// Set attribute colour
	glVertexAttribPointer(1, 4, GL_FLOAT, false, sizeof(Vertex), (void*)offsetof(Vertex, colour));
	glEnableVertexAttribArray(1);

	// Set attribute normal
	glVertexAttribPointer(2, 3, GL_FLOAT, false, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(2);

	// Instanced data
	GLuint instanceVBO;
	glGenBuffers(1, &instanceVBO);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * offsets.size(), offsets.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Sets the divisor to per instance for vertexattribute with index 3
	glVertexAttribDivisor(3, 1);

	m_NumTris = mesh.m_Indicies.size() / 3;
	m_IndiciesCount = mesh.m_Indicies.size();
	m_InstanceCount = numInstances;
}

void Renderer::UploadElementsInstanced(std::vector<std::shared_ptr<Entity>>& entities) {
	std::vector<glm::vec3> offsets;
	offsets.reserve(entities.size());
	// Like Array.prototype.map() in JS
	std::transform(entities.begin(), entities.end(),
		std::back_inserter(offsets),
		[](std::shared_ptr<Entity>& entity) {return entity->GetOffset(); });
	
	Mesh& mesh = const_cast<Mesh&>(entities.at(0)->GetMesh());
	UploadElementsInstanced(mesh, offsets, entities.size());
}

void Renderer::SetupProgram() {
	std::string vertShaderPath = "C:\\Users\\Linus\\source\\repos\\3D_Game\\3D_Game\\shaders\\vertex.glsl";
	auto vertFileContents = File::ReadFile(vertShaderPath);

	Shader vertShader(vertFileContents.c_str(), SHADER_TYPE::VERTEX);
	if (!vertShader.Compile()) {
		printf("Failed to compile Vertex Shader!\n");
		getchar();
	}

	std::string fragShaderPath = "C:\\Users\\Linus\\source\\repos\\3D_Game\\3D_Game\\shaders\\frag.glsl";
	auto fragFileContents = File::ReadFile(fragShaderPath);

	Shader fragShader(fragFileContents.c_str(), SHADER_TYPE::FRAGMENT);
	if (!fragShader.Compile()) {
		printf("Failed to compile Fragment Shader!\n");
		getchar();
	}

	std::vector<Shader*> shaders{ &vertShader, &fragShader };

	m_Program = std::make_unique<ShaderProgram>();
	if (!m_Program->Link(shaders)) {
		printf("Could not link program!\n");
	}

}

Renderer::Renderer(std::shared_ptr<Window>& window, std::shared_ptr<Camera>& cam) {
	m_Window = window;
	m_Camera = cam;
	m_Gui = std::make_shared<RendererOptionsGui>();
}

Renderer::~Renderer() {
	
}

void Renderer::Draw() {
	Timer timer("Renderer::Draw");
	//glDrawElements(GL_TRIANGLES, 6 * 3, GL_UNSIGNED_INT, 0);
	if (m_Gui->m_DisplayWireFrame)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glDrawElements(GL_TRIANGLES, m_IndiciesCount, GL_UNSIGNED_INT, 0);
}

void Renderer::DrawInstanced() {
	Timer timer("Renderer::Draw");
	//glDrawElements(GL_TRIANGLES, 6 * 3, GL_UNSIGNED_INT, 0);
	
	if (m_Gui->m_DisplayWireFrame)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//glDrawElementsInstanced(GL_TRIANGLES, m_IndiciesCount, GL_UNSIGNED_INT, 0);
	glDrawElementsInstanced(GL_TRIANGLES, m_IndiciesCount, GL_UNSIGNED_INT, 0, m_InstanceCount);
}