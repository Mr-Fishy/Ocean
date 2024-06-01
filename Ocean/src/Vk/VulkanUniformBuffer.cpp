#include "ocpch.hpp"

#include "Vk/VulkanUniformBuffer.hpp"

#include "Vk/VulkanData.hpp"

#include <chrono>

// libs
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Ocean {

	void VulkanUniformBuffer::Init() {
		CreateUniformBuffers();
	}

	void VulkanUniformBuffer::Destroy() {
		for (ui8 i = 0; i < vkData->MaxFramesInFlight; i++) {
			vkDestroyBuffer(vkData->Device.GetLogical(), v_Buffers[i], nullptr);
			vkFreeMemory(vkData->Device.GetLogical(), v_Memory[i], nullptr);
		}
	}

	void VulkanUniformBuffer::Update(ui32 currentImage) {
		static auto startTime = std::chrono::high_resolution_clock::now();

		auto currentTime = std::chrono::high_resolution_clock::now();
		f32 time = std::chrono::duration<f32, std::chrono::seconds::period>(currentTime - startTime).count();

		UniformBufferObject ubo{};

		ubo.model = glm::rotate(mat4(1.0f), time * glm::radians(90.0f), vec3(0.0f, 0.0f, 1.0f));
		ubo.view = glm::lookAt(vec3(2.0f, 2.0f, 2.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f));
		ubo.proj = glm::perspective(glm::radians(45.0f), (f32)vkData->Swapchain.GetExtent().width / (f32)vkData->Swapchain.GetExtent().height, 0.1f, 10.0f);

		ubo.proj[1][1] *= -1;

		memcpy(v_Mapped[currentImage], &ubo, sizeof(ubo));
	}

	void VulkanUniformBuffer::CreateUniformBuffers() {
		VkDeviceSize bufferSize = sizeof(UniformBufferObject);

		v_Buffers.resize(vkData->MaxFramesInFlight);
		v_Memory.resize(vkData->MaxFramesInFlight);
		v_Mapped.resize(vkData->MaxFramesInFlight);
		for (ui8 i = 0; i < v_Buffers.size(); i++) {
			vkData->Buffer.Create(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, v_Buffers[i], v_Memory[i]);

			vkMapMemory(vkData->Device.GetLogical(), v_Memory[i], 0, bufferSize, 0, &v_Mapped[i]);
		}
	}

}	// Ocean
