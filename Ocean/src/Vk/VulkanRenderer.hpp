#pragma once

#include "Core/Types.hpp"

// libs
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace Ocean {

	class VulkanRenderer {
	public:
		VulkanRenderer() : m_RenderPass(VK_NULL_HANDLE), m_PipelineLayout(VK_NULL_HANDLE), m_GraphicsPipeline(VK_NULL_HANDLE) { }
		~VulkanRenderer() = default;

		void Init();
		void Destroy() const;

		VkRenderPass GetRenderPass() const { return m_RenderPass; }
		VkPipelineLayout GetPipelineLayout() const { return m_PipelineLayout; }
		VkPipeline GetPipeline() const { return m_GraphicsPipeline; }

	private:
		void CreateRenderPass();

		void CreateGraphicsPipeline();

		/* --- */

		VkRenderPass m_RenderPass;

		VkPipelineLayout m_PipelineLayout;

		VkPipeline m_GraphicsPipeline;
	};

}	// Ocean
