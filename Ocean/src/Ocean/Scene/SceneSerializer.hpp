#pragma once

#include "Ocean/Scene/Scene.hpp"

namespace Ocean {

	class SceneSerializer {
	public:
		SceneSerializer(const Ref<Scene>& scene);

		void Serialize(const std::string& filepath);
		void SerializeRuntime(const std::string& filepath);

		bool Deserialize(const std::string& filepath);
		bool DeserializeRuntime(const std::string& filepath);

	private:
		Ref<Scene> m_Scene;

		/* --- */

		#ifdef DEBUG
			friend class Validator;
		#endif // DEBUG
	};

}	// Ocean

