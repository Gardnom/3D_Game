#pragma once
// GLM stuff
//#define GLM_EXT_INCLUDED
//#define GLM_ENABLE_EXPERIMENTAL

#include <glm/gtx/hash.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/matrix.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/integer.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Constants.h"
#include "UUID.h"

#include <vector>
#include <string>

#include <memory>

namespace Engine {
	typedef std::string HMaterial;

	/*typedef struct Material {
		glm::vec4 baseColour;
	};*/

	typedef struct Face {
		HMaterial hMaterial;
		std::vector<int> indicies;
	} Face;
}

typedef struct Vertex {
	glm::vec3 position;
	glm::vec4 colour;
	glm::vec3 normal = glm::vec3(1.0f, 1.0f, 1.0f);
} Vertex;

template<typename T>
using SPVector = std::shared_ptr<std::vector<T>>;
template<typename T>
SPVector<T> MakeSPVector() {
	return std::make_shared<std::vector<T>>();
}


typedef std::vector<glm::vec3> OffVec;

//typedef std::vector<T> PVector;