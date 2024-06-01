#include "ocpch.hpp"

#include "Vk/VulkanDescriptorSet.hpp"

#include "Vk/VulkanData.hpp"

namespace Ocean {

	void VulkanDescriptorSet::InitLayout() {
		m_DescriptorSetLayout = new VkDescriptorSetLayout;

		CreateDescriptorSetLayout();
	}

	void VulkanDescriptorSet::InitPool() {
		CreateDescriptorPool();

		CreateDescriptorSets();
	}

	void VulkanDescriptorSet::DestroyLayout() {
		vkDestroyDescriptorSetLayout(vkData->Device.GetLogical(), *m_DescriptorSetLayout, nullptr);
	
		delete m_DescriptorSetLayout;
	}

	void VulkanDescriptorSet::DestroyPool() const {
		vkDestroyDescriptorPool(vkData->Device.GetLogical(), m_DescriptorPool, nullptr);
	}

	void VulkanDescriptorSet::CreateDescriptorSetLayout() {
		VkDescriptorSetLayoutBinding uboLayout{};

		uboLayout.binding = 0;
		uboLayout.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		uboLayout.descriptorCount = 1;

		uboLayout.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

		uboLayout.pImmutableSamplers = nullptr; // Optional

		VkDescriptorSetLayoutBinding layoutBinding{};

		layoutBinding.binding = 1;
		layoutBinding.descriptorCount = 1;
		layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		layoutBinding.pImmutableSamplers = nullptr;
		layoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

		std::array<VkDescriptorSetLayoutBinding, 2> bindings = { uboLayout, layoutBinding };

		VkDescriptorSetLayoutCreateInfo info{};

		info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;

		info.bindingCount = static_cast<ui32>(bindings.size());
		info.pBindings = bindings.data();

		if (vkCreateDescriptorSetLayout(vkData->Device.GetLogical(), &info, nullptr, m_DescriptorSetLayout) != VK_SUCCESS)
			throw std::runtime_error("Failed to create Vulkan descriptor set layout!");
	}

	void VulkanDescriptorSet::CreateDescriptorPool() {
		std::array<VkDescriptorPoolSize, 2> poolSizes{};

		poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		poolSizes[0].descriptorCount = static_cast<ui32>(vkData->MaxFramesInFlight);
		poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		poolSizes[1].descriptorCount = static_cast<ui32>(vkData->MaxFramesInFlight);

		VkDescriptorPoolCreateInfo info{};

		info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;

		info.poolSizeCount = static_cast<ui32>(poolSizes.size());
		info.pPoolSizes = poolSizes.data();

		info.maxSets = static_cast<ui32>(vkData->MaxFramesInFlight);

		if (vkCreateDescriptorPool(vkData->Device.GetLogical(), &info, nullptr, &m_DescriptorPool) != VK_SUCCESS)
			throw std::runtime_error("Failed to create Vulkan descriptor pool!");
	}

	void VulkanDescriptorSet::CreateDescriptorSets() {
		std::vector<VkDescriptorSetLayout> layouts(vkData->MaxFramesInFlight, *m_DescriptorSetLayout);

		VkDescriptorSetAllocateInfo allocInfo{};

		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;

		allocInfo.descriptorPool = m_DescriptorPool;
		allocInfo.descriptorSetCount = static_cast<ui32>(vkData->MaxFramesInFlight);
		allocInfo.pSetLayouts = layouts.data();

		v_DescriptorSets.resize(vkData->MaxFramesInFlight);
		if (vkAllocateDescriptorSets(vkData->Device.GetLogical(), &allocInfo, v_DescriptorSets.data()) != VK_SUCCESS)
			throw std::runtime_error("Failed to allocate Vulkan descriptor sets!");

		for (size_t i = 0; i < vkData->MaxFramesInFlight; i++) {
			VkDescriptorBufferInfo bufferInfo{};

			bufferInfo.buffer = vkData->UniformBuffer.GetBuffers()[i];
			bufferInfo.offset = 0;
			bufferInfo.range = sizeof(UniformBufferObject);

			VkDescriptorImageInfo imageInfo{};

			imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			imageInfo.imageView = vkData->StaticTexture->GetView();
			imageInfo.sampler = vkData->Sampler.GetTextureSampler();

			std::array<VkWriteDescriptorSet, 2> descriptorWrites{};

			descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrites[0].dstSet = v_DescriptorSets[i];
			descriptorWrites[0].dstBinding = 0;
			descriptorWrites[0].dstArrayElement = 0;
			descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			descriptorWrites[0].descriptorCount = 1;
			descriptorWrites[0].pBufferInfo = &bufferInfo;

			descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrites[1].dstSet = v_DescriptorSets[i];
			descriptorWrites[1].dstBinding = 1;
			descriptorWrites[1].dstArrayElement = 0;
			descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			descriptorWrites[1].descriptorCount = 1;
			descriptorWrites[1].pImageInfo = &imageInfo;

			vkUpdateDescriptorSets(vkData->Device.GetLogical(), static_cast<ui32>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
		}
	}

}	// Ocean
