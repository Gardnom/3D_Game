#pragma once
#include "core.h"
#include <vector>
#include <unordered_map>
#include "Material.h"

class Mesh {
public:
	std::vector<Vertex> m_Vertices;
	std::vector<int> m_Indicies;
	std::unordered_map<std::string, Material> m_Materials;
};