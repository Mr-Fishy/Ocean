#pragma once

// libs
#include <glm/glm.hpp>

namespace Ocean {

	namespace Math {

		bool DecomposeTransform(const glm::mat4& transform, glm::vec3& outTranslation, glm::vec3& outRotation, glm::vec3& outScale);

	}	// Math

}	// Ocean
