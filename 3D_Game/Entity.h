#pragma once
#include "core.h"
#include "Mesh.h"

class Entity {
public:
	Entity();
	Entity(Mesh& m_Mesh, glm::vec3 offset);

	~Entity();
	const glm::vec3& GetOffset();
	const Mesh& GetMesh();

	bool m_Visible = true;
	Engine::UUID m_ID;
	
	std::string m_Name;

private: 
	Mesh m_Mesh;
	glm::vec3 m_Offset = glm::vec3(0.0f, 0.0f, 0.0f);
};