#include "MaterialSystem.h"


namespace Engine {
	std::unordered_map<Engine::HMaterial, PMaterial> MaterialSystem::m_Materials;
	
	PMaterial MaterialSystem::GetMaterial(Engine::HMaterial hMaterial) {
		auto it = m_Materials.find(hMaterial);
		if (it == m_Materials.end()) return nullptr;
		return (*it).second;
	}
	
	bool MaterialSystem::AddMaterial(Engine::Material& mat) {
		m_Materials.emplace(mat.m_Name, std::make_shared<Material>(mat));
		return true;
	}
}