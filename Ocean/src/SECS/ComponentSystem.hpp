#pragma once

#include "BaseComponentSystem.hpp"

namespace SECS {

	using std::tuple;
	using std::add_pointer_t;

	template <class... Comps>
	class ComponentSystem : public BaseComponentSystem {
	public:
		virtual ~ComponentSystem() = default;
		virtual bool Init() = 0;
		virtual void Update(double deltaTime) = 0;

	protected:
		using CompTuple = tuple<add_pointer_t<Comps>...>;		// A tuple of component pointers
		// vector<CompTuple, int> m_Components;
	};

}	// SECS