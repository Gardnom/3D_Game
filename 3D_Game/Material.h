#pragma once

#include "core.h"

namespace Engine {
	class Material {
	public:
		Material() {

		}
		Material(std::string& name) {
			m_Name = name;
		}
		std::string m_Name;
		glm::vec4 m_Colour;
	};
}