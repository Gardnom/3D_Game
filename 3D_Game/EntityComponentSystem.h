#pragma once
#include "UUID.h"

#include "core.h"
#include <unordered_map>
#include "Mesh.h"
#include "Entity.h"
#include <memory>


namespace Engine {
	namespace ECS {
	typedef std::vector<UUID> EntityIdentifiers;
	typedef std::shared_ptr<Entity> PEntity;

		class Ecs {
		public:
			// MeshID -> Vector containing Id for entities with associated mesh
			static std::unordered_map<UUID, EntityIdentifiers> m_Meshes;

			// List of all entities in the scene
			static std::unordered_map<UUID, PEntity> m_Entities;

			static PEntity AddEntity(Entity& entity);
			
			static PEntity GetEntity(UUID id);
		};

	}
}