#include "Entity.h"

Entity::Entity() {

}

Entity::~Entity() {

}

Entity::Entity(Mesh& mesh, glm::vec3 offset) {
	m_Mesh = mesh;
	m_Offset = offset;
}

const glm::vec3& Entity::GetOffset() {
	return m_Offset;
}

const Mesh& Entity::GetMesh() {
	return m_Mesh;
}