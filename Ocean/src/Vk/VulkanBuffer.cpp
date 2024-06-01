#include "ocpch.hpp"

#include "Vk/VulkanBuffer.hpp"

#include "Vk/VulkanData.hpp"

namespace Ocean {

	void VulkanBuffer::InitPool() {
		CreateCommandPool();
	}

	void VulkanBuffer::InitBuffers() {
		CreateCommandBuffer();
	}

	void VulkanBuffer::Destroy() const {
		vkDestroyCommandPool(vkData->Device.GetLogical(), m_CommandPool, nullptr);
	}

	void VulkanBuffer::Create(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags props, VkBuffer& buffer, VkDeviceMemory& memory) {
		VkBufferCreateInfo info{};

		info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;

		info.size = size;
		info.usage = usage;

		info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if (vkCreateBuffer(vkData->Device.GetLogical(), &info, nullptr, &buffer) != VK_SUCCESS)
			throw std::runtime_error("Failed to create Vulkan vertex buffer!");

		VkMemoryRequirements memReq;
		vkGetBufferMemoryRequirements(vkData->Device.GetLogical(), buffer, &memReq);

		VkMemoryAllocateInfo alloc{};

		alloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		alloc.allocationSize = memReq.size;
		alloc.memoryTypeIndex = VkUtils::FindMemoryType(memReq.memoryTypeBits, props, vkData->Device.GetPhysical());

		if (vkAllocateMemory(vkData->Device.GetLogical(), &alloc, nullptr, &memory) != VK_SUCCESS)
			throw std::runtime_error("Failed to allocate Vulkan vertex buffer memory!");

		vkBindBufferMemory(vkData->Device.GetLogical(), buffer, memory, 0);
	}

	void VulkanBuffer::Copy(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
		VkCommandBuffer commandBuffer = BeginSingleCommand();

		VkBufferCopy copy{};

		copy.srcOffset = 0; // Optional
		copy.dstOffset = 0; // Optional
		copy.size = size;

		vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copy);

		EndSingleCommand(commandBuffer);
	}

	VkCommandBuffer VulkanBuffer::BeginSingleCommand() const {
		VkCommandBufferAllocateInfo info{};

		info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		info.commandPool = m_CommandPool;
		info.commandBufferCount = 1;

		VkCommandBuffer commandBuffer;
		vkAllocateCommandBuffers(vkData->Device.GetLogical(), &info, &commandBuffer);

		VkCommandBufferBeginInfo beginInfo{};

		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		vkBeginCommandBuffer(commandBuffer, &beginInfo);

		return commandBuffer;
	}

	void VulkanBuffer::EndSingleCommand(VkCommandBuffer commandBuffer) const {
		vkEndCommandBuffer(commandBuffer);

		VkSubmitInfo submit{};

		submit.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submit.commandBufferCount = 1;
		submit.pCommandBuffers = &commandBuffer;

		vkQueueSubmit(vkData->GraphicsQueue, 1, &submit, VK_NULL_HANDLE);
		vkQueueWaitIdle(vkData->GraphicsQueue);

		vkFreeCommandBuffers(vkData->Device.GetLogical(), m_CommandPool, 1, &commandBuffer);
	}

	void VulkanBuffer::RecordBuffer(VkCommandBuffer commandBuffer, ui32 imageIndex) {
		VkCommandBufferBeginInfo info{};

		info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		info.flags = 0;						// Optional
		info.pInheritanceInfo = nullptr;	// Optional

		if (vkBeginCommandBuffer(commandBuffer, &info) != VK_SUCCESS)
			throw std::runtime_error("Failed to begin recording Vulkan command buffer!");

		VkRenderPassBeginInfo renderInfo{};

		renderInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderInfo.renderPass = vkData->Renderer.GetRenderPass();
		renderInfo.framebuffer = vkData->Framebuffer.GetBuffers()[imageIndex];

		renderInfo.renderArea.offset = { 0, 0 };
		renderInfo.renderArea.extent = vkData->Swapchain.GetExtent();

		std::array<VkClearValue, 2> clearValues;
		clearValues[0].color = { {0.0f, 0.0f, 0.0f, 1.0f} };
		clearValues[1].depthStencil = { 1.0f, 0 };

		renderInfo.clearValueCount = static_cast<ui32>(clearValues.size());
		renderInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(commandBuffer, &renderInfo, VK_SUBPASS_CONTENTS_INLINE);


		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vkData->Renderer.GetPipeline());

		VkViewport viewport{};

		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<f32>(vkData->Swapchain.GetExtent().width);
		viewport.height = static_cast<f32>(vkData->Swapchain.GetExtent().height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

		VkRect2D scissor{};

		scissor.offset = { 0, 0 };
		scissor.extent = vkData->Swapchain.GetExtent();

		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

		VkBuffer vertexBuffers[] = { vkData->VertexBuffer.GetBuffer() };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

		vkCmdBindIndexBuffer(commandBuffer, vkData->IndexBuffer.GetBuffer(), 0, VK_INDEX_TYPE_UINT16);

		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vkData->Renderer.GetPipelineLayout(), 0, 1, &vkData->Descriptor.GetSets()[vkData->CurrentFrame], 0, nullptr);

		vkCmdDrawIndexed(commandBuffer, static_cast<ui32>(vkData->IndexBuffer.GetIndices().size()), 1, 0, 0, 0);


		vkCmdEndRenderPass(commandBuffer);

		if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
			throw std::runtime_error("Failed to record Vulkan command buffer!");
	}

	void VulkanBuffer::CreateCommandPool() {
		QueueFamilyIndices queueFamilyIndices = QueueFamilyIndices::FindQueueFamilies(vkData->Device.GetPhysical(), vkData->Surface);

		VkCommandPoolCreateInfo info{};

		info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		info.queueFamilyIndex = queueFamilyIndices.GraphicsFamily.value();

		if (vkCreateCommandPool(vkData->Device.GetLogical(), &info, nullptr, &m_CommandPool) != VK_SUCCESS)
			throw std::runtime_error("Failed to create Vulkan command pool!");
	}

	void VulkanBuffer::CreateCommandBuffer() {
		v_CommandBuffers.resize(vkData->MaxFramesInFlight);

		VkCommandBufferAllocateInfo info{};

		info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		info.commandPool = m_CommandPool;
		info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		info.commandBufferCount = static_cast<ui32>(v_CommandBuffers.size());

		if (vkAllocateCommandBuffers(vkData->Device.GetLogical(), &info, v_CommandBuffers.data()) != VK_SUCCESS)
			throw std::runtime_error("Failed to allocate Vulkan command buffers!");
	}

}	// Ocean
