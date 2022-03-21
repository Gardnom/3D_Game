#include "EntityComponentSystem.h"

namespace Engine {
	namespace ECS {

		std::unordered_map<UUID, EntityIdentifiers> Ecs::m_Meshes;

		// List of all entities in the scene
		std::unordered_map<UUID, PEntity> Ecs::m_Entities;

		std::shared_ptr<Entity> Ecs::AddEntity(Entity& entity) {
			UUID id = generate_uuid_v4();
			PEntity pEntity = std::make_shared<Entity>(entity);
			pEntity->m_ID = id;

			m_Entities.emplace(id, pEntity);

			return pEntity;
		}

		std::shared_ptr<Entity> Ecs::GetEntity(UUID id) {
			auto it = m_Entities.find(id);
			if (it == m_Entities.end()) {
				return nullptr;
			}
			
			return it->second;
		}
	}
}