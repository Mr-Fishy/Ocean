#include "Device.hpp"

#include "Renderer/Infos.hpp"
#include "Renderer/Renderer.hpp"
#include "Renderer/Shader.hpp"
#include "Renderer/SwapChain.hpp"
#include "Renderer/Buffer.hpp"

// libs
#include <GLFW/glfw3.h>

// std
#include <set>

namespace Ocean {

	namespace Vulkan {

		struct SwapChainSupportDetails {
			VkSurfaceCapabilitiesKHR Capabilities;

			std::vector<VkSurfaceFormatKHR> Formats;
			std::vector<VkPresentModeKHR> PresentModes;

		};	// SwapChainSupportDetails



		const std::vector<Vertex> tempVertices = {
				{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
				{{ 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
				{{ 0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}},
				{{-0.5f,  0.5f}, {1.0f, 1.0f, 1.0f}}
		};

		const std::vector<u16> indices = {
			0, 1, 2, 2, 3, 0
		};



		void Device::Init(DeviceConfig* config) {
			p_Allocator = config->allocator;
			p_Renderer = config->renderer;
			p_WindowHandle = config->windowPtr;

			// Create the window surface
			CHECK_RESULT(
				glfwCreateWindowSurface(p_Renderer->GetVulkanInstance(), (GLFWwindow*)p_WindowHandle, nullptr, &m_Surface),
				"Failed to create window surface!"
			);

			u32 deviceCount = 0;
			vkEnumeratePhysicalDevices(p_Renderer->GetVulkanInstance(), &deviceCount, nullptr);

			OASSERTM(deviceCount != 0, "Failed to find GPUs with Vulkan support!");

			std::vector<VkPhysicalDevice> devices(deviceCount);
			vkEnumeratePhysicalDevices(p_Renderer->GetVulkanInstance(), &deviceCount, devices.data());

			for (const auto& device : devices) {
				if (IsDeviceSuitable(device)) {
					m_Physical = device;

					break;
				}
			}

			if (m_Physical == VK_NULL_HANDLE)
				OASSERTM(false, "Failed to find a suitable GPU!\n");

			QueueFamilyIndices indices = FindQueueFamilies(m_Physical);
			CreateLogicalDevice(indices);

			vkGetDeviceQueue(m_Device, indices.GraphicsFamily.value(), 0, &m_GraphicsQueue);
			vkGetDeviceQueue(m_Device, indices.PresentFamily.value(), 0, &m_PresentQueue);

			CreateCommandPool(indices);

			CreateVertexBuffer();
			CreateIndexBuffer();

			CreateCommandBuffers();
		}

		void Device::Shutdown() {
			m_CommandBuffers.Shutdown();

			p_VertexBuffer->Shutdown();
			ofree(p_VertexBuffer, p_Allocator);

			p_IndexBuffer->Shutdown();
			ofree(p_IndexBuffer, p_Allocator);

			vkDestroyCommandPool(m_Device, m_CommandPool, nullptr);

			vkDestroySurfaceKHR(p_Renderer->GetVulkanInstance(), m_Surface, nullptr);

			vkDestroyDevice(m_Device, nullptr);
		}

		b8 Device::IsDeviceSuitable(VkPhysicalDevice device) {
			QueueFamilyIndices indices = FindQueueFamilies(device);

			b8 extensionsSupported = CheckDeviceExtensionSupport(device);

			SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(device);

			b8 swapChainAdequate = false;
			if (extensionsSupported)
				swapChainAdequate = !swapChainSupport.Formats.empty() && !swapChainSupport.PresentModes.empty();

			/*VkPhysicalDeviceFeatures deviceFeatures;
			vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

			VkPhysicalDeviceProperties deviceProperties;
			vkGetPhysicalDeviceProperties(device, &deviceProperties);*/

			return indices.IsComplete() && extensionsSupported && swapChainAdequate;
		}

		b8 Device::CheckDeviceExtensionSupport(VkPhysicalDevice device) {
			u32 extensionCount;
			vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

			std::vector<VkExtensionProperties> availableExtensions(extensionCount);
			vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

			i32  requiredExtensionCount = ArraySize(k_DeviceExtensions);
			for (const auto& extension : availableExtensions) {
				for (u8 i = 0; i < ArraySize(k_DeviceExtensions); i++) {
					if (strcmp(extension.extensionName, k_DeviceExtensions[i]) == 0) {
						requiredExtensionCount--;

						break;
					}
				}

				if (requiredExtensionCount == 0)
					return true;
			}

			return false;
		}

		QueueFamilyIndices Device::FindQueueFamilies(VkPhysicalDevice device) const {
			QueueFamilyIndices indices;

			u32 familyCount = 0;
			vkGetPhysicalDeviceQueueFamilyProperties(device, &familyCount, nullptr);

			FixedArray<VkQueueFamilyProperties> families(familyCount);
			vkGetPhysicalDeviceQueueFamilyProperties(device, &familyCount, families.Data());
			families.SetSize(familyCount);

			b8 canBeShared = false;
			for (u32 i = 0; i < familyCount; i++) {
				if (families.Get(i).queueFlags & VK_QUEUE_GRAPHICS_BIT)
					indices.GraphicsFamily = i;

				VkBool32 presentSupport = false;
				vkGetPhysicalDeviceSurfaceSupportKHR(device, i, m_Surface, &presentSupport);

				if (presentSupport) {
					if (indices.GraphicsFamily.value() == i)
						canBeShared = true;
					else
						indices.PresentFamily = i;
				}

				if (indices.IsComplete())
					break;
			}

			families.Shutdown();

			if (!indices.PresentFamily.has_value() && canBeShared)
				indices.PresentFamily.value() = indices.GraphicsFamily.value();

			return indices;
		}

		void Device::CreateLogicalDevice(QueueFamilyIndices indices) {
			std::vector<VkDeviceQueueCreateInfo> queueInfos;
			std::array<u32, QueueFamilyIndices::UniqueFamilyCount> uniqueQueueFamilies = {
				indices.GraphicsFamily.value(),
				indices.PresentFamily.value()
			};

			f32 queuePriority = 1.0f;
			for (u32 queueFamily : uniqueQueueFamilies) {
				VkDeviceQueueCreateInfo queueInfo{ };
				queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;

				queueInfo.queueFamilyIndex = queueFamily;

				queueInfo.queueCount = 1;
				queueInfo.pQueuePriorities = &queuePriority;

				queueInfos.emplace_back(queueInfo);
			}

			VkPhysicalDeviceFeatures deviceFeatures{ };
			vkGetPhysicalDeviceFeatures(m_Physical, &deviceFeatures);

			VkDeviceCreateInfo createInfo{ };
			createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

			createInfo.queueCreateInfoCount = (u32)queueInfos.size();
			createInfo.pQueueCreateInfos = queueInfos.data();

			createInfo.pEnabledFeatures = &deviceFeatures;

			createInfo.enabledExtensionCount = ArraySize(k_DeviceExtensions);
			createInfo.ppEnabledExtensionNames = k_DeviceExtensions;

			// Newer versions of Vulkan do not require this.
			createInfo.enabledLayerCount = 0;
			createInfo.ppEnabledLayerNames = nullptr;

			CHECK_RESULT(
				vkCreateDevice(m_Physical, &createInfo, nullptr, &m_Device),
				"Failed to create logical device!"
			);
		}

		SwapChainSupportDetails Device::QuerySwapChainSupport(VkPhysicalDevice device) const {
			SwapChainSupportDetails details;

			vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, m_Surface, &details.Capabilities);

			u32 formatCount;
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_Surface, &formatCount, nullptr);

			if (formatCount != 0) {
				details.Formats.resize(formatCount);

				vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_Surface, &formatCount, details.Formats.data());
			}

			u32 presentModeCount;
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_Surface, &presentModeCount, nullptr);

			if (presentModeCount != 0) {
				details.PresentModes.resize(presentModeCount);

				vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_Surface, &presentModeCount, details.PresentModes.data());
			}

			return details;
		}

		void Device::CreateCommandPool(QueueFamilyIndices indices) {
			VkCommandPoolCreateInfo info{ };
			info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;

			info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

			info.queueFamilyIndex = indices.GraphicsFamily.value();

			CHECK_RESULT(
				vkCreateCommandPool(m_Device, &info, nullptr, &m_CommandPool),
				"Failed to create command pool!"
			);
		}

		void Device::CreateVertexBuffer() {
			BufferConfig config = {
				sizeof(Vertex) * tempVertices.size(),
				VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
				this
			};

			Buffer* stagingBuffer = oallocat(Buffer, 1, p_Allocator);
			stagingBuffer->Init(&config);

			stagingBuffer->SubmitData(config.size, (void*)tempVertices.data());

			config.usageFlags = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
			config.memoryFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

			p_VertexBuffer = oallocat(Buffer, 1, p_Allocator);
			p_VertexBuffer->Init(&config);

			CopyBuffer(stagingBuffer, p_VertexBuffer, config.size);

			stagingBuffer->Shutdown();
			ofree(stagingBuffer, p_Allocator);
		}

		void Device::CreateIndexBuffer() {
			BufferConfig config = {
				sizeof(u16) * indices.size(),
				VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
				this
			};

			Buffer* stagingBuffer = oallocat(Buffer, 1, p_Allocator);
			stagingBuffer->Init(&config);

			stagingBuffer->SubmitData(config.size, (void*)indices.data());

			config.usageFlags = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
			config.memoryFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

			p_IndexBuffer = oallocat(Buffer, 1, p_Allocator);
			p_IndexBuffer->Init(&config);

			CopyBuffer(stagingBuffer, p_IndexBuffer, config.size);

			stagingBuffer->Shutdown();
			ofree(stagingBuffer, p_Allocator);
		}

		void Device::CreateCommandBuffers() {
			m_CommandBuffers.Init(p_Renderer->GetMaxFramesInFlight());
			m_CommandBuffers.SetSize(p_Renderer->GetMaxFramesInFlight());

			VkCommandBufferAllocateInfo info{ };
			info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;

			info.commandPool = m_CommandPool;
			/*
			 * VK_COMMAND_BUFFER_LEVEL_PRIMARY: Can be submitted to a queue for execution, but cannot be called from other command buffers.
			 *
			 * VK_COMMAND_BUFFER_LEVEL_SECONDARY: Cannot be submitted directly, but can be called from primary command buffers.
			 */
			info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
			info.commandBufferCount = m_CommandBuffers.Size();

			CHECK_RESULT(
				vkAllocateCommandBuffers(m_Device, &info, m_CommandBuffers.Data()),
				"Failed to allocate command buffers!"
			);
		}

		void Device::FlushCommandBuffer(u8 frame) {
			vkResetCommandBuffer(m_CommandBuffers.Get(frame), 0);
		}

		void Device::RecordCommandBuffer(u32 imageIndex, u8 frame) {
			VkCommandBuffer commandBuffer = m_CommandBuffers.Get(frame);

			VkCommandBufferBeginInfo beginInfo{ };
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

			/**
			 * VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT: The command buffer will be re-recorded directly after executing it once.
			 *
			 * VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT: The command buffer is a seconday buffer that will be used entirely within a single render pass.
			 *
			 * VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT: The command buffer can be resubmitted while it is also already pending execution.
			 */
			beginInfo.flags = 0;
			/**
			 * pInheritanceInfo is only relevant for secondary command buffers.
			 * It specifies which state to inherit from the primary command buffers.
			 */
			beginInfo.pInheritanceInfo = nullptr;

			CHECK_RESULT(
				vkBeginCommandBuffer(commandBuffer, &beginInfo),
				"Failed to begin recording command buffer!"
			);

			VkRenderPassBeginInfo renderInfo{ };
			renderInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;

			renderInfo.renderPass = p_Renderer->GetRenderPass();

			renderInfo.framebuffer = p_SwapChain->GetFramebuffer(imageIndex);

			renderInfo.renderArea.offset = { 0, 0 };
			renderInfo.renderArea.extent = p_SwapChain->GetExtent();

			VkClearValue clearColor = { { {  0.0f, 0.0f, 0.0f, 1.0f } } };// GetClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			renderInfo.clearValueCount = 1;
			renderInfo.pClearValues = &clearColor;

			/**
			 * VK_SUBPASS_CONTENTS_INLINE: The render pass commands will be embedded in the primary command buffer itself and no secondary command buffers will be executed.
			 *
			 * VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS: The render pass commands will be executed from secondary command buffers.
			 */
			vkCmdBeginRenderPass(commandBuffer, &renderInfo, VK_SUBPASS_CONTENTS_INLINE);

				/**
				 *
				 */
				vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, p_Renderer->GetGraphicsPipeline());

				VkViewport viewport{ };
				viewport.x = viewport.y = 0.0f;
				viewport.width = (f32)p_SwapChain->GetExtent().width;
				viewport.height = (f32)p_SwapChain->GetExtent().height;
				viewport.minDepth = 0.0f;
				viewport.maxDepth = 1.0f;

				vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

				VkRect2D scissor{ };
				scissor.offset = { 0, 0 };
				scissor.extent = p_SwapChain->GetExtent();

				vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

				VkBuffer vertexBuffers[] = { p_VertexBuffer->GetBuffer() };
				VkDeviceSize offsets[] = { 0 };

				vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

				vkCmdBindIndexBuffer(commandBuffer, p_IndexBuffer->GetBuffer(), 0, VK_INDEX_TYPE_UINT16);

				// TODO: Convert Vertex Buffer and Index Buffer into a single buffer with offsets. (Use vkCmdBindVertexBuffers()).
				vkCmdDrawIndexed(commandBuffer, (u32)indices.size(), 1, 0, 0, 0);

			vkCmdEndRenderPass(commandBuffer);

			CHECK_RESULT(
				vkEndCommandBuffer(commandBuffer),
				"Failed to record command buffer!"
			);
		}

		void Device::SubmitCommandBuffer(const SyncObjects& syncObjects, u8 frame) {
			VkSubmitInfo info{ };
			info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

			VkSemaphore waitSemaphores[] = { syncObjects.Sempahores.PresentComplete };
			VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

			info.waitSemaphoreCount = 1;
			info.pWaitSemaphores = waitSemaphores;

			info.pWaitDstStageMask = waitStages;

			VkCommandBuffer commandBuffers[] = { m_CommandBuffers.Get(frame) };

			info.commandBufferCount = 1;
			info.pCommandBuffers = commandBuffers;

			VkSemaphore signalSemaphores[] = { syncObjects.Sempahores.RenderComplete };

			info.signalSemaphoreCount = 1;
			info.pSignalSemaphores = signalSemaphores;

			CHECK_RESULT(
				vkQueueSubmit(m_GraphicsQueue, 1, &info, syncObjects.Fences.InFlight),
				"Failed to submit draw command buffer!"
			);
		}

		u32 Device::GetMemoryType(u32 typeFilter, VkMemoryPropertyFlags properties) {
			VkPhysicalDeviceMemoryProperties memProperties;
			vkGetPhysicalDeviceMemoryProperties(m_Physical, &memProperties);

			for (u32 i = 0; i < memProperties.memoryTypeCount; i++) {
				if (typeFilter & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
					return i;
			}

			OASSERTM(false, "Failed to find suitable memory type!");
		}

		void Device::CopyBuffer(Buffer* src, Buffer* dst, u32 size) {
			VkCommandBufferAllocateInfo allocInfo{};
			allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;

			allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

			allocInfo.commandPool = m_CommandPool;
			allocInfo.commandBufferCount = 1;

			// TODO: Create a Memory Transfer Command Pool
			VkCommandBuffer commandBuffer;
			vkAllocateCommandBuffers(m_Device, &allocInfo, &commandBuffer);

			VkCommandBufferBeginInfo beginInfo{
				VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
				nullptr, // pNext
				VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
				nullptr  // pInheritance
			};
			vkBeginCommandBuffer(commandBuffer, &beginInfo);

			VkBufferCopy copyRegion{ };
			copyRegion.srcOffset = 0;
			copyRegion.dstOffset = 0;
			copyRegion.size = size;

			vkCmdCopyBuffer(commandBuffer, src->GetBuffer(), dst->GetBuffer(), 1, &copyRegion);

			vkEndCommandBuffer(commandBuffer);

			VkSubmitInfo submitInfo{ };
			submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

			submitInfo.commandBufferCount = 1;
			submitInfo.pCommandBuffers = &commandBuffer;

			vkQueueSubmit(m_GraphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
			vkQueueWaitIdle(m_GraphicsQueue);

			vkFreeCommandBuffers(m_Device, m_CommandPool, 1, &commandBuffer);
		}

	}	// Vulkan

}	// Ocean
