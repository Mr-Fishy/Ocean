#pragma once

namespace SECS {

	class EntityManager;
	class Entity;

	class BaseComponentSystem {
	public:
		virtual ~BaseComponentSystem() = default;

		virtual void OnEntityCreated(const Entity& entity) = 0;
		virtual void OnEntityDestroyed(const Entity& entity) = 0;

		virtual void Update(double deltaTime) = 0;

	protected:
		EntityManager* m_pEntityManager;
	};

}	// SECS