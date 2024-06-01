#include "ocpch.hpp"

#include "Vk/VulkanContext.hpp"

#include "Ocean/Logger/Log.hpp"

namespace Ocean {

	VulkanData* vkData = nullptr;

	VulkanContext::VulkanContext(GLFWwindow* window) {
		static VulkanData* data = new VulkanData(ContextSpecs("Ocean App", 2, window));
		vkData = data;

		vkData->Init();
	}

	VulkanContext::~VulkanContext() {
		vkData->Destroy();

		delete vkData;
	}

	void VulkanContext::DrawFrame() {
		vkWaitForFences(vkData->Device.GetLogical(), 1, &vkData->Sync.GetInFlight()[vkData->CurrentFrame], VK_TRUE, UINT64_MAX);

		ui32 imageIndex;
		VkResult result;
		result = vkAcquireNextImageKHR(vkData->Device.GetLogical(), vkData->Swapchain.GetSwapchain(), UINT64_MAX, vkData->Sync.GetImageAvailable()[vkData->CurrentFrame], VK_NULL_HANDLE, &imageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR) {
			vkData->Swapchain.RecreateSwapchain();
			return;
		}
		else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
			throw std::runtime_error("Failed to acquire Vulkan swap chain image!");

		vkData->UniformBuffer.Update(vkData->CurrentFrame);

		vkResetFences(vkData->Device.GetLogical(), 1, &vkData->Sync.GetInFlight()[vkData->CurrentFrame]);

		vkResetCommandBuffer(*vkData->Buffer.GetBuffer(vkData->CurrentFrame), /* VkCommandBufferResetFlags */ 0);
		vkData->Buffer.RecordBuffer(*vkData->Buffer.GetBuffer(vkData->CurrentFrame), imageIndex);

		VkSubmitInfo info{};

		info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore waitSemaphores[] = { vkData->Sync.GetImageAvailable()[vkData->CurrentFrame] };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

		info.waitSemaphoreCount = 1;
		info.pWaitSemaphores = waitSemaphores;
		info.pWaitDstStageMask = waitStages;

		info.commandBufferCount = 1;
		info.pCommandBuffers = vkData->Buffer.GetBuffer(vkData->CurrentFrame); // TODO: FIX

		VkSemaphore signalSemaphores[] = { vkData->Sync.GetRenderFinished()[vkData->CurrentFrame] };

		info.signalSemaphoreCount = 1;
		info.pSignalSemaphores = signalSemaphores;

		if (vkQueueSubmit(vkData->GraphicsQueue, 1, &info, vkData->Sync.GetInFlight()[vkData->CurrentFrame]) != VK_SUCCESS)
			throw std::runtime_error("Failed to submit Vulkan draw command buffer!");

		VkPresentInfoKHR presentInfo{};

		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;

		VkSwapchainKHR swapChains[] = { vkData->Swapchain.GetSwapchain() };

		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;

		presentInfo.pImageIndices = &imageIndex;

		presentInfo.pResults = nullptr; // Optional

		result = vkQueuePresentKHR(vkData->PresentQueue, &presentInfo);

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_FramebufferResized) {
			m_FramebufferResized = false;
			vkData->Swapchain.RecreateSwapchain();
		}
		else if (result != VK_SUCCESS)
			throw std::runtime_error("Failed to present Vulkan swap chain image!");

		vkData->CurrentFrame = (vkData->CurrentFrame + 1) % 2;
	}

	VkDevice VulkanContext::GetDevice() const { return vkData->Device.GetLogical(); }

}
