#pragma once

#include "Core/Types.hpp"

// libs
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace Ocean {

	/**
	 * @brief Descriptor sets are Vulkan's form of defining an information structure for shaders.
	 */
	class VulkanDescriptorSet {
	public:
		VulkanDescriptorSet() : m_DescriptorSetLayout(nullptr), m_DescriptorPool(VK_NULL_HANDLE) { }
		~VulkanDescriptorSet() = default;

		void InitLayout();
		void InitPool();
		void DestroyLayout();
		void DestroyPool() const;

		VkDescriptorSetLayout* GetLayout() const { return m_DescriptorSetLayout; }

		std::vector<VkDescriptorSet> GetSets() const { return v_DescriptorSets; }

	private:
		void CreateDescriptorSetLayout();

		void CreateDescriptorPool();
		void CreateDescriptorSets();

		/* --- */

		VkDescriptorSetLayout* m_DescriptorSetLayout;

		VkDescriptorPool m_DescriptorPool;
		std::vector<VkDescriptorSet> v_DescriptorSets;
	};

}	// Ocean
