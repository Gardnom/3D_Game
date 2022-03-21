#pragma once
#include "core.h"
#include <vector>
#include <unordered_map>
#include "UUID.h"

class Mesh {
public:
	Mesh() {
		m_ID = Engine::generate_uuid_v4();
	}
	std::vector<Vertex> m_Vertices;
	std::vector<int> m_Indicies;
	Engine::UUID m_ID;
	//std::unordered_map<std::string, Material> m_Materials;
};