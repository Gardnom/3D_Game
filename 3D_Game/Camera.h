#pragma once
#include "core.h"

class Camera {
	/*glm::vec3 m_Eye = glm::vec3(0.0f, 0.0f, 20.0f);
	glm::vec3 m_Up = glm::vec3(0.0f, 1.0f, 0.0f);
	const glm::vec3 m_Center = glm::vec3(0.0f, 0.0f, 0.0f);*/
public:
	Camera() {
		
	}

	glm::mat4 GetViewMatrix() {
		return glm::lookAt(m_Position, m_Position + GetDirection(), m_Up);
	}
	
	glm::vec3 GetDirection() {
		glm::vec3 direction;
		direction.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		direction.y = sin(glm::radians(m_Pitch));
		direction.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		return direction;
	}

	glm::vec3 m_Up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 m_Position = glm::vec3(0.0f, 0.0f, 20.0f);
	glm::vec3 m_Front = glm::vec3(0.0f, 0.0f, -1.0f);

	float m_Yaw = -90.0f;
	float m_Pitch = 0.0f;
	
private:
};