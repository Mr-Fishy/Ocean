#pragma once

// libs
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace Ocean {

	class VulkanSync {
	public:
		VulkanSync() = default;
		~VulkanSync() = default;

		void Init();
		void Destroy();

		std::vector<VkSemaphore> GetImageAvailable() const { return v_ImageAvailableSemaphores; }
		std::vector<VkSemaphore> GetRenderFinished() const { return v_RenderFinsishedSemaphores; }

		std::vector<VkFence> GetInFlight() const { return v_InFlightFences; }

	private:
		void CreateSyncObjects();

		/* --- */

		std::vector<VkSemaphore> v_ImageAvailableSemaphores;
		std::vector<VkSemaphore> v_RenderFinsishedSemaphores;

		std::vector<VkFence> v_InFlightFences;
	};

}	// Ocean
