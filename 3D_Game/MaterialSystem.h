#pragma once
#include "core.h"
#include "Material.h"
#include <memory>
#include <unordered_map>


namespace Engine {
	typedef std::shared_ptr<Engine::Material> PMaterial;
	class MaterialSystem {
	public:
		static PMaterial GetMaterial(Engine::HMaterial hMaterial);
		static bool AddMaterial(Engine::Material& mat);
		static bool AddMaterial(std::string prefix, Engine::Material& mat);
		static std::unordered_map<Engine::HMaterial, PMaterial> m_Materials;
	};
}