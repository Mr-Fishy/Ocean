#include "Renderer.hpp"

#include "Ocean/Core/Types/glmTypes.hpp"

#include "Ocean/Core/Primitives/Time.hpp"
#include "Ocean/Core/Primitives/glmMath.hpp"

#include "Ocean/Core/Window.hpp"

#include "Renderer/Infos.hpp"
#include "Renderer/Device.hpp"
#include "Renderer/SwapChain.hpp"
#include "Renderer/Framebuffer.hpp"

// libs
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

			m_Width = renConfig->Width;
			m_Height = renConfig->Height;

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

			DeviceConfig devConfig(
				p_Allocator,
				this,
				renConfig->MainWindow->Handle()
			);

			p_Device = oallocat(Device, 1, p_Allocator);
			p_Device->Init(&devConfig);

			SwapChainConfig swapConfig(
				p_Allocator,
				this,
				p_Device,
				renConfig->MainWindow->Handle()
			);

			p_SwapChain = oallocat(SwapChain, 1, p_Allocator);
			p_SwapChain->Init(&swapConfig);
			p_SwapChain->CreateSwapChain(&m_Width, &m_Height);

			p_Device->SetSwapChain(p_SwapChain);

			CreateRenderPass();
			CreateDescriptorSetLayout();
			CreateGraphicsPipeline();

			CreateUniformBuffers();

			p_Device->CreateDescriptors();

			p_SwapChain->CreateFramebuffers();

			CreateSyncObjects();
		}

		void Renderer::Shutdown() {
			vkDestroyPipeline(p_Device->GetLogical(), m_GraphicsPipeline, nullptr);
			vkDestroyPipelineLayout(p_Device->GetLogical(), m_PipelineLayout, nullptr);
			vkDestroyRenderPass(p_Device->GetLogical(), m_RenderPass, nullptr);

			for (u8 i = 0; i < m_MaxFramesInFlight; i++) {
				vkDestroySemaphore(p_Device->GetLogical(), m_SyncObjects.Get(i).Sempahores.PresentComplete, nullptr);
				vkDestroySemaphore(p_Device->GetLogical(), m_SyncObjects.Get(i).Sempahores.RenderComplete, nullptr);

				vkDestroyFence(p_Device->GetLogical(), m_SyncObjects.Get(i).Fences.InFlight, nullptr);
			}
			m_SyncObjects.Shutdown();

			p_SwapChain->Shutdown();
			ofree(p_SwapChain, p_Allocator);

			for (u8 i = 0; i < m_MaxFramesInFlight; i++) {
				m_UniformBuffers.Get(i).UBO.Shutdown();
			}
			m_UniformBuffers.Shutdown();

			vkDestroyDescriptorSetLayout(p_Device->GetLogical(), m_DescriptorSetLayout, nullptr);

			p_Device->Shutdown();
			ofree(p_Device, p_Allocator);

		#ifdef OC_DEBUG

			DestroyDebugUtilsMessengerEXT(m_Instance, m_DebugMessenger, nullptr);

		#endif

			vkDestroyInstance(m_Instance, nullptr);
		}



		void Renderer::BeginFrame() {
		}

		void Renderer::RenderFrame() {
			vkWaitForFences(p_Device->GetLogical(), 1, &m_SyncObjects.Get(m_Frame).Fences.InFlight, VK_TRUE, u64_max);
			vkResetFences(p_Device->GetLogical(), 1, &m_SyncObjects.Get(m_Frame).Fences.InFlight);

			u32 imageIndex;
			p_SwapChain->GetNextImage(
				m_SyncObjects.Get(m_Frame).Sempahores.PresentComplete,
				&imageIndex
			);

			p_Device->FlushCommandBuffer(m_Frame);
			p_Device->RecordCommandBuffer(imageIndex, m_Frame);

			UpdateUniformBuffer(m_Frame);

			p_Device->SubmitCommandBuffer(m_SyncObjects.Get(m_Frame), m_Frame);

			p_SwapChain->QueuePresentation(
				p_Device->GetPresentationQueue(),
				imageIndex,
				m_SyncObjects.Get(m_Frame).Sempahores.RenderComplete
			);

			m_Frame = (m_Frame + 1) % m_MaxFramesInFlight;
		}

		void Renderer::EndFrame() {
		}

		void Renderer::CleanUp() {
			vkDeviceWaitIdle(p_Device->GetLogical());
		}

		f32 Renderer::AspectRatio() const {
			return f32();
		}

		void Renderer::ResizeSwapchain(i32 width, i32 height) {
			vkDeviceWaitIdle(p_Device->GetLogical());

			while (width == 0 || height == 0) {
				glfwGetFramebufferSize((GLFWwindow*)p_Device->GetWindowHandle(), &width, &height);
				glfwWaitEvents();
			}

			m_Width = width;
			m_Height = height;
			p_SwapChain->RecreateSwapChain(&m_Width, &m_Height);
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

		void Renderer::CreateRenderPass() {
			VkAttachmentDescription colorAttachment = GetColorAttachmentDescription(p_SwapChain->GetColorFormat());

			VkSubpassDescription subpass = GetSubpassAttachmentDescription();

			VkSubpassDependency dependency = GetSubpassDependency();

			VkRenderPassCreateInfo info{ };
			info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;

			info.attachmentCount = 1;
			info.pAttachments = &colorAttachment;

			info.subpassCount = 1;
			info.pSubpasses = &subpass;

			info.dependencyCount = 1;
			info.pDependencies = &dependency;

			CHECK_RESULT(
				vkCreateRenderPass(p_Device->GetLogical(), &info, nullptr, &m_RenderPass),
				"Failed to create render pass!"
			);
		}

		void Renderer::CreateDescriptorSetLayout() {
			VkDescriptorSetLayoutBinding layout{ };
			layout.binding = 0;
			
			layout.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			layout.descriptorCount = 1;

			layout.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
			// This is only relevant for image sampling related descriptors.
			layout.pImmutableSamplers = nullptr; // Optional

			VkDescriptorSetLayoutCreateInfo info{ };
			info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;

			info.bindingCount = 1;
			info.pBindings = &layout;

			CHECK_RESULT(
				vkCreateDescriptorSetLayout(p_Device->GetLogical(), &info, nullptr, &m_DescriptorSetLayout),
				"Failed to create descriptor set layout!"
			);
		}

		void Renderer::CreateGraphicsPipeline() {
			Shader* shaders = oallocat(Shader, 2, p_Allocator);
			shaders[0].Init(p_Allocator, "src/Shaders/vert.spv");
			shaders[1].Init(p_Allocator, "src/Shaders/frag.spv");

			VkPipelineShaderStageCreateInfo shaderStages[] = {
				GetVertextShaderStageInfo(&shaders[0], p_Device->GetLogical(), "main"),
				GetFragmentShaderStageInfo(&shaders[1], p_Device->GetLogical(), "main")
			};

			auto description = Vertex::GetBindingDescription();
			auto attributes = Vertex::GetAttributeDescriptions();

			VkPipelineVertexInputStateCreateInfo vertexInfo{ };
			vertexInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
			
			vertexInfo.vertexBindingDescriptionCount = 1;
			vertexInfo.pVertexBindingDescriptions = &description;
			
			vertexInfo.vertexAttributeDescriptionCount = attributes.Size();
			vertexInfo.pVertexAttributeDescriptions = attributes.Data();

			VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
			inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;

			inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

			inputAssembly.primitiveRestartEnable = VK_FALSE;

			VkPipelineViewportStateCreateInfo viewportState{ };
			viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;

			viewportState.viewportCount = 1;
			viewportState.scissorCount = 1;

			VkPipelineRasterizationStateCreateInfo rasterizer{};
			rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;

			rasterizer.depthClampEnable = VK_FALSE;
			rasterizer.depthBiasEnable = VK_FALSE;

			rasterizer.rasterizerDiscardEnable = VK_FALSE;

			rasterizer.polygonMode = VK_POLYGON_MODE_FILL;

			rasterizer.lineWidth = 1.0f;

			rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
			rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;

			VkPipelineMultisampleStateCreateInfo multisampling{};
			multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;

			multisampling.sampleShadingEnable = VK_FALSE;

			multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

			VkPipelineColorBlendAttachmentState colorBlendAttachment{};
			colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT
												| VK_COLOR_COMPONENT_G_BIT
												| VK_COLOR_COMPONENT_B_BIT
												| VK_COLOR_COMPONENT_A_BIT
				;
			
			colorBlendAttachment.blendEnable = VK_FALSE;

			VkPipelineColorBlendStateCreateInfo colorBlending{};
			colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;

			colorBlending.logicOpEnable = VK_FALSE;
			colorBlending.logicOp = VK_LOGIC_OP_COPY;

			colorBlending.attachmentCount = 1;
			colorBlending.pAttachments = &colorBlendAttachment;

			colorBlending.blendConstants[0] = 0.0f;
			colorBlending.blendConstants[1] = 0.0f;
			colorBlending.blendConstants[2] = 0.0f;
			colorBlending.blendConstants[3] = 0.0f;

			VkDynamicState dynamicStates[] = {
				VK_DYNAMIC_STATE_VIEWPORT,
				VK_DYNAMIC_STATE_SCISSOR
			};

			VkPipelineDynamicStateCreateInfo dynamicInfo{ };
			dynamicInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;

			dynamicInfo.dynamicStateCount = ArraySize(dynamicStates);
			dynamicInfo.pDynamicStates = dynamicStates;

			VkPipelineLayoutCreateInfo layoutInfo{ };
			layoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

			layoutInfo.setLayoutCount = 1;
			layoutInfo.pSetLayouts = &m_DescriptorSetLayout;

			layoutInfo.pushConstantRangeCount = 0;

			CHECK_RESULT(
				vkCreatePipelineLayout(p_Device->GetLogical(), &layoutInfo, nullptr, &m_PipelineLayout),
				"Failed to create pipeline layout!"
			);

			VkGraphicsPipelineCreateInfo pipelineInfo{ };
			pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;

			pipelineInfo.stageCount = 2;
			pipelineInfo.pStages = shaderStages;

			pipelineInfo.pVertexInputState = &vertexInfo;
			pipelineInfo.pInputAssemblyState = &inputAssembly;
			pipelineInfo.pViewportState = &viewportState;
			pipelineInfo.pRasterizationState = &rasterizer;
			pipelineInfo.pMultisampleState = &multisampling;
			pipelineInfo.pColorBlendState = &colorBlending;
			pipelineInfo.pDynamicState = &dynamicInfo;

			pipelineInfo.layout = m_PipelineLayout;
			
			pipelineInfo.renderPass = m_RenderPass;
			pipelineInfo.subpass = 0;

			pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

			CHECK_RESULT(
				vkCreateGraphicsPipelines(p_Device->GetLogical(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_GraphicsPipeline),
				"Failed to create graphics pipeline!"
			);

			shaders[0].Shutdown();
			shaders[1].Shutdown();
			ofree(shaders, p_Allocator);
		}

		void Renderer::CreateSyncObjects() {
			m_SyncObjects.Init(m_MaxFramesInFlight);
			m_SyncObjects.SetSize(m_MaxFramesInFlight);

			VkSemaphoreCreateInfo semaInfo{ };
			semaInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

			VkFenceCreateInfo fenceInfo{ };
			fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
			fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

			const VkDevice deviceRef = p_Device->GetLogical();
			for (u8 i = 0; i < m_MaxFramesInFlight; i++) {
				CHECK_RESULT(
					vkCreateSemaphore(deviceRef, &semaInfo, nullptr, &m_SyncObjects.Get(i).Sempahores.PresentComplete),
					"Failed to create presentation semaphore!"
				);
				CHECK_RESULT(
					vkCreateSemaphore(deviceRef, &semaInfo, nullptr, &m_SyncObjects.Get(i).Sempahores.RenderComplete),
					"Failed to create render semaphore!"
				);

				CHECK_RESULT(
					vkCreateFence(deviceRef, &fenceInfo, nullptr, &m_SyncObjects.Get(i).Fences.InFlight),
					"Failed to create in-flight fence!"
				);
			}
		}

		void Renderer::CreateUniformBuffers() {
			m_UniformBuffers.Init(m_MaxFramesInFlight);

			BufferConfig config{ };
			config.device = p_Device;
			config.size = sizeof(UniformBufferObject);

			config.usageFlags = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
			config.memoryFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

			m_UniformBuffers.SetSize(m_MaxFramesInFlight);
			for (u32 i = 0; i < m_MaxFramesInFlight; i++) {
				m_UniformBuffers.Get(i).UBO.Init(&config);

				m_UniformBuffers.Get(i).Data = m_UniformBuffers.Get(i).UBO.GetMappedMemory(config.size);
			}
		}

		void Renderer::UpdateUniformBuffer(u8 frame) {
			static auto startTime = oTimeNow();

			f32 time = (f32)oTimeFromRealiSec(startTime);

			UniformBufferObject ubo{ };
			ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			ubo.view  = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			ubo.proj  = glm::perspective(glm::radians(45.0f), p_SwapChain->GetExtent().width / (f32)p_SwapChain->GetExtent().height, 0.1f, 10.0f);

			ubo.proj[1][1] *= -1;

			memcpy(m_UniformBuffers.Get(frame).Data, &ubo, sizeof(UniformBufferObject));
		}

	}	// Vulkan

}	// Ocean
