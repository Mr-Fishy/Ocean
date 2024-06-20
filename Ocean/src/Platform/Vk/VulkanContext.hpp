#pragma once

#include "Ocean/Renderer/GraphicsContext.hpp"

// libs
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>

namespace Ocean::Vk {

	class VulkanContext : public GraphicsContext {
	public:
		VulkanContext(GLFWwindow* window);

		virtual void Init() override final;
		virtual void SwapBuffers() override final;

	private:
		void CreateInstance();

		/* --- */

		GLFWwindow* m_Window;

		VkInstance m_Instance;
	};

}	// Ocean::Vk
