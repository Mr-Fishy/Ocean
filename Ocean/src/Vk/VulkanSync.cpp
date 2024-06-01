#include "ocpch.hpp"

#include "Vk/VulkanSync.hpp"

#include "Vk/VulkanData.hpp"

namespace Ocean {

	void VulkanSync::Init() {
		CreateSyncObjects();
	}

	void VulkanSync::Destroy() {
		for (ui8 i = 0; i < vkData->MaxFramesInFlight; i++) {
			vkDestroySemaphore(vkData->Device.GetLogical(), v_RenderFinsishedSemaphores[i], nullptr);
			vkDestroySemaphore(vkData->Device.GetLogical(), v_ImageAvailableSemaphores[i], nullptr);
			vkDestroyFence(vkData->Device.GetLogical(), v_InFlightFences[i], nullptr);
		}
	}

	void VulkanSync::CreateSyncObjects() {
		VkSemaphoreCreateInfo semaInfo{};

		semaInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo fenceInfo{};

		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		v_ImageAvailableSemaphores.resize(vkData->MaxFramesInFlight);
		v_RenderFinsishedSemaphores.resize(vkData->MaxFramesInFlight);
		v_InFlightFences.resize(vkData->MaxFramesInFlight);
		for (ui8 i = 0; i < vkData->MaxFramesInFlight; i++) {
			if (vkCreateSemaphore(vkData->Device.GetLogical(), &semaInfo, nullptr, &v_ImageAvailableSemaphores[i]) != VK_SUCCESS
				|| vkCreateSemaphore(vkData->Device.GetLogical(), &semaInfo, nullptr, &v_RenderFinsishedSemaphores[i]) != VK_SUCCESS
				|| vkCreateFence(vkData->Device.GetLogical(), &fenceInfo, nullptr, &v_InFlightFences[i]) != VK_SUCCESS)
				throw std::runtime_error("Failed to create Vulkan semaphores!");
		}
	}

}	// Ocean
