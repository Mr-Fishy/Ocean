#pragma once

#include "Ocean/Core/Timestep.hpp"

// libs
#include "entt.hpp"

namespace Ocean {

	class Scene {
	public:
		Scene();
		~Scene();

		entt::entity CreateEntity();

		// TEMP
		entt::registry& Reg() { return m_Registry; }

		void OnUpdate(Timestep ts);

	private:
		entt::registry m_Registry;
	};

}	// Ocean