#include "VulkanDevice.hpp"

#include "Renderer/VulkanRenderer.hpp"

#include <GLFW/glfw3.h>

// std
#include <optional>
#include <set>

namespace Ocean {

	namespace Vulkan {

		struct QueueFamilyIndices {
			std::optional<u32> GraphicsFamily;
			std::optional<u32> PresentFamily;

			static constexpr u8 UniqueFamilyCount = 2;

			b8 IsComplete() const {
				return GraphicsFamily.has_value() && PresentFamily.has_value();
			}

		};	// QueueFamilyIndices

		struct SwapChainSupportDetails {
			VkSurfaceCapabilitiesKHR Capabilities;

			std::vector<VkSurfaceFormatKHR> Formats;
			std::vector<VkPresentModeKHR> PresentModes;

		};	// SwapChainSupportDetails


		void Device::Init(DeviceConfig* config) {
			p_Allocator = config->MemAllocator;
			p_Renderer = config->Renderer;

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

			CreateSwapChain();

			CreateImageViews();

			CreateRenderPass();

			CreateGraphicsPipeline();
		}

		void Device::Shutdown() {
			vkDestroyPipeline(m_Device, m_GraphicsPipeline, nullptr);
			vkDestroyPipelineLayout(m_Device, m_PipelineLayout, nullptr);
			vkDestroyRenderPass(m_Device, m_RenderPass, nullptr);

			for (u32 i = 0; i < m_SwapChainViews.Size(); i++)
				vkDestroyImageView(m_Device, *m_SwapChainViews.Get(i), nullptr);

			vkDestroySwapchainKHR(m_Device, m_SwapChain, nullptr);

			vkDestroyDevice(m_Device, nullptr);

			m_SwapChainViews.Shutdown();
			m_SwapChainImages.Shutdown();
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

		QueueFamilyIndices Device::FindQueueFamilies(VkPhysicalDevice device) {
			QueueFamilyIndices indices;

			u32 familyCount = 0;
			vkGetPhysicalDeviceQueueFamilyProperties(device, &familyCount, nullptr);

			std::vector<VkQueueFamilyProperties> families(familyCount);
			vkGetPhysicalDeviceQueueFamilyProperties(device, &familyCount, families.data());

			for (u32 i = 0; i < familyCount; i++) {
				if (families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
					indices.GraphicsFamily = i;

				VkBool32 presentSupport = false;
				vkGetPhysicalDeviceSurfaceSupportKHR(device, i, p_Renderer->GetVulkanSurface(), &presentSupport);

				if (presentSupport)
					indices.PresentFamily = i;

				if (indices.IsComplete())
					break;
			}

			return indices;
		}

		void Device::CreateLogicalDevice(QueueFamilyIndices indices) {
			std::vector<VkDeviceQueueCreateInfo> queueInfos;
			std::array<u32, QueueFamilyIndices::UniqueFamilyCount> uniqueQueueFamilies = {
				indices.GraphicsFamily.value(),
				indices.PresentFamily.value()
			};

			queueInfos.reserve(uniqueQueueFamilies.size());
			f32 queuePriority = 1.0f;
			for (u32 queueFamily : uniqueQueueFamilies) {
				VkDeviceQueueCreateInfo queueInfo{ };
				queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;

				queueInfo.queueFamilyIndex = queueFamily;

				queueInfo.queueCount = 1;
				queueInfo.pQueuePriorities = &queuePriority;

				queueInfos.push_back(queueInfo);
			}

			VkPhysicalDeviceFeatures deviceFeatures;
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

			CheckResultSuccess(vkCreateDevice(m_Physical, &createInfo, nullptr, &m_Device),
							   "Failed to create logical device!");
		}

		SwapChainSupportDetails Device::QuerySwapChainSupport(VkPhysicalDevice device) {
			SwapChainSupportDetails details;
			VkSurfaceKHR surface = p_Renderer->GetVulkanSurface();

			vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.Capabilities);

			u32 formatCount;
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

			if (formatCount != 0) {
				details.Formats.resize(formatCount);

				vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.Formats.data());
			}

			u32 presentModeCount;
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

			if (presentModeCount != 0) {
				details.PresentModes.resize(presentModeCount);

				vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.PresentModes.data());
			}

			return details;
		}

		VkSurfaceFormatKHR Device::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
			for (const auto& format : availableFormats) {
				if (format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
					return format;
			}

			// An assumption that the first is likely good enough
			return availableFormats[0];
		}

		VkPresentModeKHR Device::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availableModes) {
			for (const auto& mode : availableModes) {
				if (mode == VK_PRESENT_MODE_MAILBOX_KHR)
					return mode;
			}

			// This is the only guaranteed mode available
			return VK_PRESENT_MODE_FIFO_KHR;
		}

		VkExtent2D Device::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
			if (capabilities.currentExtent.width != std::numeric_limits<u32>::max()) {
				return capabilities.currentExtent;
			}
			else {
				i32 width, height;
				glfwGetFramebufferSize((GLFWwindow*)p_WindowHandle, &width, &height);

				VkExtent2D extent = {
					(u32)width, (u32)height
				};

				extent.width = std::clamp(extent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
				extent.height = std::clamp(extent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

				return extent;
			}
		}

		void Device::CreateSwapChain() {
			SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(m_Physical);

			VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.Formats);
			VkPresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.PresentModes);
			VkExtent2D extent = ChooseSwapExtent(swapChainSupport.Capabilities);

			u32 imageCount = swapChainSupport.Capabilities.minImageCount + 1;

			// if maxImageCount = 0 then there is no maximum
			if (imageCount > swapChainSupport.Capabilities.maxImageCount && swapChainSupport.Capabilities.maxImageCount != 0)
				imageCount = swapChainSupport.Capabilities.maxImageCount;

			VkSwapchainCreateInfoKHR info{ };
			info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;

			info.surface = p_Renderer->GetVulkanSurface();

			info.minImageCount = imageCount;

			info.imageFormat = surfaceFormat.format;
			info.imageColorSpace = surfaceFormat.colorSpace;

			info.imageExtent = extent;

			info.imageArrayLayers = 1;
			info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

			QueueFamilyIndices indices = FindQueueFamilies(m_Physical);
			u32 queueFamilyIndices[] = {
				indices.GraphicsFamily.value(),
				indices.PresentFamily.value()
			};

			if (indices.GraphicsFamily.value() != indices.PresentFamily.value()) {
				info.imageSharingMode = VK_SHARING_MODE_CONCURRENT; // TODO: Change to EXCLUSIVE for better performance

				info.queueFamilyIndexCount = 2;
				info.pQueueFamilyIndices = queueFamilyIndices;
			}
			else {
				info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;

				info.queueFamilyIndexCount = 0; // Optional
				info.pQueueFamilyIndices = nullptr; // Optional
			}

			info.preTransform = swapChainSupport.Capabilities.currentTransform;

			info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

			info.presentMode = presentMode;
			info.clipped = VK_TRUE;

			info.oldSwapchain = VK_NULL_HANDLE;

			CheckResultSuccess(vkCreateSwapchainKHR(m_Device, &info, nullptr, &m_SwapChain),
							   "Failed to create swap chain!");

			vkGetSwapchainImagesKHR(m_Device, m_SwapChain, &imageCount, nullptr);

			m_SwapChainImages.Init(p_Allocator, imageCount);
			vkGetSwapchainImagesKHR(m_Device, m_SwapChain, &imageCount, m_SwapChainImages.Data());

			m_SwapChainFormat = surfaceFormat.format;
			m_SwapChainExtent = extent;
		}

		void Device::CreateImageViews() {
			m_SwapChainViews.Init(p_Allocator, m_SwapChainImages.Size());

			for (u32 i = 0; i < m_SwapChainViews.Size(); i++) {
				VkImageViewCreateInfo info{ };
				info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;

				info.image = *m_SwapChainImages.Get(i);

				info.viewType = VK_IMAGE_VIEW_TYPE_2D;
				info.format = m_SwapChainFormat;

				info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
				info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
				info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
				info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

				info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				info.subresourceRange.baseMipLevel = 0;
				info.subresourceRange.levelCount = 1;
				info.subresourceRange.baseArrayLayer = 0;
				info.subresourceRange.layerCount = 1;

				CheckResultSuccess(vkCreateImageView(m_Device, &info, nullptr, m_SwapChainViews.Get(i)),
								   "Failed to create image views!");
			}
		}

		void Device::CreateRenderPass() {
			VkAttachmentDescription colorAttachment{ };
			colorAttachment.format = m_SwapChainFormat;
			colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;

			colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;

			colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

			colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

			VkAttachmentReference colorRef{ };
			colorRef.attachment = 0;
			colorRef.layout = VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL;

			VkSubpassDescription subpass{ };
			subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

			subpass.colorAttachmentCount = 1;
			subpass.pColorAttachments = &colorRef;

			VkRenderPassCreateInfo renderInfo{ };
			renderInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;

			renderInfo.attachmentCount = 1;
			renderInfo.pAttachments = &colorAttachment;

			renderInfo.subpassCount = 1;
			renderInfo.pSubpasses = &subpass;

			CheckResultSuccess(vkCreateRenderPass(m_Device, &renderInfo, nullptr, &m_RenderPass),
							   "Failed to create render pass!");
		}

		void Device::CreateGraphicsPipeline() {
			m_VertShader.Init(p_Allocator, "Shaders/vert.spv");
			m_FragShader.Init(p_Allocator, "Shaders/frag.spv");

			VkPipelineShaderStageCreateInfo vertShaderInfo{ };
			vertShaderInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;

			vertShaderInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
			vertShaderInfo.module = m_VertShader.GetShaderModule(m_Device);

			vertShaderInfo.pName = "main";
			vertShaderInfo.pSpecializationInfo = nullptr;

			VkPipelineShaderStageCreateInfo fragShaderInfo{ };
			fragShaderInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;

			fragShaderInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
			fragShaderInfo.module = m_FragShader.GetShaderModule(m_Device);

			fragShaderInfo.pName = "main";
			fragShaderInfo.pSpecializationInfo = nullptr;

			VkPipelineShaderStageCreateInfo shaderStages[] = {
				vertShaderInfo,
				fragShaderInfo
			};

			VkPipelineVertexInputStateCreateInfo inputInfo{ };
			inputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

			inputInfo.vertexBindingDescriptionCount = 0;
			inputInfo.pVertexBindingDescriptions = nullptr;

			inputInfo.vertexAttributeDescriptionCount = 0;
			inputInfo.pVertexAttributeDescriptions = nullptr;

			VkPipelineInputAssemblyStateCreateInfo assemblyInfo{ };
			assemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;

			assemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			assemblyInfo.primitiveRestartEnable = VK_FALSE;

			VkViewport viewport{ };
			viewport.x = viewport.y = 0.0f;
			
			viewport.width = (f32)m_SwapChainExtent.width;
			viewport.height = (f32)m_SwapChainExtent.height;

			viewport.minDepth = 0.0f;
			viewport.maxDepth = 1.0f;

			VkRect2D scissor{ };
			scissor.offset = { 0, 0 };
			scissor.extent = m_SwapChainExtent;

			std::array<VkDynamicState, 2> dynamicStates = {
				VK_DYNAMIC_STATE_VIEWPORT,
				VK_DYNAMIC_STATE_SCISSOR
			};

			VkPipelineDynamicStateCreateInfo dynamicInfo{ };
			dynamicInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;

			dynamicInfo.dynamicStateCount = (u32)dynamicStates.size();
			dynamicInfo.pDynamicStates = dynamicStates.data();

			VkPipelineViewportStateCreateInfo viewportInfo{ };
			viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;

			viewportInfo.viewportCount = 1;
			viewportInfo.pViewports = &viewport;

			viewportInfo.scissorCount = 1;
			viewportInfo.pScissors = &scissor;

			VkPipelineRasterizationStateCreateInfo rasterizerInfo{ };
			rasterizerInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;

			rasterizerInfo.depthClampEnable = VK_FALSE;
			rasterizerInfo.rasterizerDiscardEnable = VK_FALSE;

			rasterizerInfo.polygonMode = VK_POLYGON_MODE_FILL;
			rasterizerInfo.lineWidth = 1.0f;

			rasterizerInfo.cullMode = VK_CULL_MODE_BACK_BIT;
			rasterizerInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;

			rasterizerInfo.depthBiasEnable = VK_FALSE;
			rasterizerInfo.depthBiasConstantFactor = 0.0f;
			rasterizerInfo.depthBiasClamp = 0.0f;
			rasterizerInfo.depthBiasSlopeFactor = 0.0f;

			VkPipelineMultisampleStateCreateInfo multisamplingInfo{ };
			multisamplingInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;

			multisamplingInfo.sampleShadingEnable = VK_FALSE;
			multisamplingInfo.minSampleShading = 1.0f;
			multisamplingInfo.pSampleMask = nullptr;

			multisamplingInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

			multisamplingInfo.alphaToCoverageEnable = VK_FALSE;
			multisamplingInfo.alphaToOneEnable = VK_FALSE;

			VkPipelineColorBlendAttachmentState colorAttachment{ };
			colorAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
											 VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
			colorAttachment.blendEnable = VK_FALSE;

			colorAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
			colorAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
			colorAttachment.colorBlendOp = VK_BLEND_OP_ADD;

			colorAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
			colorAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
			colorAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

			VkPipelineColorBlendStateCreateInfo colorState{ };
			colorState.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;

			colorState.logicOpEnable = VK_FALSE;
			colorState.logicOp = VK_LOGIC_OP_COPY; // Optional

			colorState.attachmentCount = 1;
			colorState.pAttachments = &colorAttachment;

			colorState.blendConstants[0] = 0.0f;
			colorState.blendConstants[1] = 0.0f;
			colorState.blendConstants[2] = 0.0f;
			colorState.blendConstants[3] = 0.0f;

			VkPipelineLayoutCreateInfo layoutInfo{ };
			layoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

			layoutInfo.setLayoutCount = 0;
			layoutInfo.pSetLayouts = nullptr;

			layoutInfo.pushConstantRangeCount = 0;
			layoutInfo.pPushConstantRanges = nullptr;

			CheckResultSuccess(vkCreatePipelineLayout(m_Device, &layoutInfo, nullptr, &m_PipelineLayout),
							   "Failed to create pipeline layout!");

			VkGraphicsPipelineCreateInfo pipelineInfo{ };
			pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;

			pipelineInfo.stageCount = 2;
			pipelineInfo.pStages = shaderStages;

			pipelineInfo.pVertexInputState = &inputInfo;
			pipelineInfo.pInputAssemblyState = &assemblyInfo;
			pipelineInfo.pViewportState = &viewportInfo;
			pipelineInfo.pRasterizationState = &rasterizerInfo;
			pipelineInfo.pMultisampleState = &multisamplingInfo;
			pipelineInfo.pDepthStencilState = nullptr;
			pipelineInfo.pColorBlendState = &colorState;
			pipelineInfo.pDynamicState = &dynamicInfo;

			pipelineInfo.layout = m_PipelineLayout;

			pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
			pipelineInfo.basePipelineIndex = -1;

			CheckResultSuccess(vkCreateGraphicsPipelines(m_Device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_GraphicsPipeline),
							   "Failed to create graphics pipeline!");

			m_VertShader.DestroyShader(m_Device);
			m_VertShader.DestroyShader(m_Device);
		}

	}	// Vulkan

}	// Ocean
