
#include "ocpch.hpp"
#include "Entity.hpp"

namespace Ocean {

	Entity::Entity(entt::entity handle, Scene* scene) : m_EntityHandle(handle), m_Scene(scene) { }

}	// Ocean
