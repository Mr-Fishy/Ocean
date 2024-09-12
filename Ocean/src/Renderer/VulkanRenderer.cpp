#include "VulkanRenderer.hpp"

#include "Ocean/Core/Window.hpp"

#include "Renderer/VulkanInfos.hpp"

#include "Renderer/VulkanDevice.hpp"
#include "Renderer/VulkanSwapChain.hpp"

#include <GLFW/glfw3.h>

namespace Ocean {

	namespace Vulkan {

		Renderer s_Renderer;

		Renderer* Renderer::Instance() {
			return &s_Renderer;
		}

		void Renderer::Init(void* config) {
			RendererConfig* renConfig = (RendererConfig*)config;
			p_Allocator = renConfig->MemAllocator;

			VkApplicationInfo appInfo{ };
			appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;

			appInfo.pApplicationName = renConfig->AppName;
			appInfo.applicationVersion = VK_MAKE_API_VERSION(1, renConfig->AppVersionMajor, renConfig->AppVersionMinor, renConfig->AppVersionPatch);

			appInfo.pEngineName = "Ocean Engine";
			appInfo.engineVersion = VK_MAKE_API_VERSION(1, 1, 0, 0);

			appInfo.apiVersion = VK_API_VERSION_1_3;

			VkInstanceCreateInfo createInfo{ };
			createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
			createInfo.pApplicationInfo = &appInfo;

			auto extensions = GetRequiredExtensions();
			createInfo.enabledExtensionCount = (u32)extensions.size();
			createInfo.ppEnabledExtensionNames = extensions.data();

		#ifdef OC_DEBUG

			OASSERTM(CheckValidationSupport(), "Validation layers requested, but not available!");

			createInfo.enabledLayerCount = (u32)k_ValidationLayers.size();
			createInfo.ppEnabledLayerNames = k_ValidationLayers.data();

			VkDebugUtilsMessengerCreateInfoEXT debugInfo{ };
			SetDebugMessengerInfo(debugInfo);

			createInfo.pNext = &debugInfo;

		#else

			createInfo.enabledLayerCount = 0;
			createInfo.ppEnabledLayerNames = nullptr;

			createInfo.pNext = nullptr;

		#endif

			CHECK_RESULT(
				vkCreateInstance(&createInfo, nullptr, &m_Instance),
				"Failed to create instance!"
			);

		#ifdef OC_DEBUG

			CreateDebugMessenger();

		#endif

			DeviceConfig devConfig{ p_Allocator, this, renConfig->MainWindow->Handle() };
			p_Device = (Device*)p_Allocator->Allocate(sizeof(Device), alignof(Device));
			p_Device->Init(&devConfig);

			SwapChainConfig swapConfig(p_Allocator, this, p_Device, renConfig->MainWindow->Handle());
			p_SwapChain = (SwapChain*)p_Allocator->Allocate(sizeof(SwapChain), alignof(SwapChain));
			p_SwapChain->Init(&swapConfig);

			CreateRenderPass();
			CreateGraphicsPipeline();
		}

		void Renderer::Shutdown() {
			for (u32 i = 0; i < m_Framebuffers.Size(); i++)
				m_Framebuffers.Get(i).Shutdown();

			vkDestroyPipeline(p_Device->GetLogical(), m_GraphicsPipeline, nullptr);
			vkDestroyPipelineLayout(p_Device->GetLogical(), m_PipelineLayout, nullptr);
			vkDestroyRenderPass(p_Device->GetLogical(), m_RenderPass, nullptr);

			vkDestroySemaphore(p_Device->GetLogical(), m_SyncObjects.Sempahores.PresentComplete, nullptr);
			vkDestroySemaphore(p_Device->GetLogical(), m_SyncObjects.Sempahores.RenderComplete, nullptr);

			vkDestroyFence(p_Device->GetLogical(), m_SyncObjects.Fences.InFlight, nullptr);

			p_SwapChain->Shutdown();
			p_Device->Shutdown();
			p_Allocator->Deallocate(p_Device);

		#ifdef OC_DEBUG

			DestroyDebugUtilsMessengerEXT(m_Instance, m_DebugMessenger, nullptr);

		#endif

			vkDestroySurfaceKHR(m_Instance, m_Surface, nullptr);
			vkDestroyInstance(m_Instance, nullptr);
		}

		void Renderer::BeginFrame() {
		}

		void Renderer::EndFrame() {
		}

		f32 Renderer::AspectRatio() const {
			return f32();
		}

		void Renderer::ResizeSwapchain(u32 width, u32 height) {
		}

		b8 Renderer::CheckValidationSupport() {
			u32 layerCount;
			vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

			std::vector<VkLayerProperties> availableLayers(layerCount);
			vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

			for (const char* layerName : k_ValidationLayers) {
				b8 layerFound = false;

				for (const auto& layerProperties : availableLayers) {
					if (strcmp(layerName, layerProperties.layerName) == 0) {
						layerFound = true;
						break;
					}
				}

				if (!layerFound) {
					return false;
				}
			}

			return true;
		}

		std::vector<cstring> Renderer::GetRequiredExtensions() {
			u32 glfwExtensionCount = 0;
			cstring* glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

			std::vector<cstring> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

		#ifdef OC_DEBUG

			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

		#endif

			return extensions;
		}

		void Renderer::SetDebugMessengerInfo(VkDebugUtilsMessengerCreateInfoEXT& info) {
			info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;

			info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
				VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
				VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

			info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
				VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
				VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

			info.pfnUserCallback = oDebugCallback;
			info.pUserData = nullptr; // Optional
		}

		void Renderer::CreateDebugMessenger() {
			VkDebugUtilsMessengerCreateInfoEXT info{ };
			SetDebugMessengerInfo(info);

			CHECK_RESULT(
				CreateDebugUtilsMessengerEXT(m_Instance, &info, nullptr, &m_DebugMessenger),
				"Failed to set up debug messenger!"
			);
		}

		void Renderer::CreateSyncObjects() {
			VkSemaphoreCreateInfo semaInfo{ };
			semaInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

			VkFenceCreateInfo fenceInfo{ };
			fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
			fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

			const VkDevice deviceRef = p_Device->GetLogical();
			CHECK_RESULT(
				vkCreateSemaphore(deviceRef, &semaInfo, nullptr, &m_SyncObjects.Sempahores.PresentComplete),
				"Failed to create presentation semaphore!"
			);
			CHECK_RESULT(
				vkCreateSemaphore(deviceRef, &semaInfo, nullptr, &m_SyncObjects.Sempahores.RenderComplete),
				"Failed to create render semaphore!"
			);

			CHECK_RESULT(
				vkCreateFence(deviceRef, &fenceInfo, nullptr, &m_SyncObjects.Fences.InFlight),
				"Failed to create in-flight fence!"
			);
		}

		void Renderer::CreateRenderPass() {
			VkAttachmentDescription colorAttachment = ColorAttachmentDescription(p_SwapChain->GetColorFormat());

			VkSubpassDescription subpass = SubpassAttachmentDescription();

			VkRenderPassCreateInfo renderInfo{ };
			renderInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;

			renderInfo.attachmentCount = 1;
			renderInfo.pAttachments = &colorAttachment;

			renderInfo.subpassCount = 1;
			renderInfo.pSubpasses = &subpass;

			CHECK_RESULT(
				vkCreateRenderPass(p_Device->GetLogical(), &renderInfo, nullptr, &m_RenderPass),
				"Failed to create render pass!"
			);
		}

		void Renderer::CreateGraphicsPipeline() {
			m_VertShader.Init(p_Allocator, "src/Shaders/vert.spv");
			m_FragShader.Init(p_Allocator, "src/Shaders/frag.spv");

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

			CHECK_RESULT(
				vkCreatePipelineLayout(m_Device, &layoutInfo, nullptr, &m_PipelineLayout),
				"Failed to create pipeline layout!"
			);

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

			pipelineInfo.renderPass = m_RenderPass;
			pipelineInfo.subpass = 0;

			pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
			pipelineInfo.basePipelineIndex = -1;

			CHECK_RESULT(
				vkCreateGraphicsPipelines(m_Device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_GraphicsPipeline),
				"Failed to create graphics pipeline!"
			);

			m_VertShader.DestroyShader(m_Device);
			m_FragShader.DestroyShader(m_Device);
		}

	}	// Vulkan

}	// Ocean
