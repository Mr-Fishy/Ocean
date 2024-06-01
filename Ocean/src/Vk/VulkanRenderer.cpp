#include "ocpch.hpp"

#include "Vk/VulkanRenderer.hpp"

#include "Vk/VulkanData.hpp"

// std
#include <fstream>

namespace Ocean {

	void VulkanRenderer::Init() {
		CreateRenderPass();

		vkData->Descriptor.InitLayout();

		CreateGraphicsPipeline();
	}

	void VulkanRenderer::Destroy() const {
		vkDestroyPipeline(vkData->Device.GetLogical(), m_GraphicsPipeline, nullptr);

		vkDestroyPipelineLayout(vkData->Device.GetLogical(), m_PipelineLayout, nullptr);

		vkDestroyRenderPass(vkData->Device.GetLogical(), m_RenderPass, nullptr);
	}

	/* HELPER FUNCTIONS */

	static std::vector<char> ReadFile(const std::string& filename) {
		std::ifstream file(filename, std::ios::ate | std::ios::binary);

		if (!file.is_open())
			throw std::runtime_error("Failed to open SPIR-V shader: " + filename);

		ui64 fileSize = (ui64)file.tellg();
		std::vector<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);

		file.close();

		return buffer;
	}

	static VkShaderModule CreateShaderModule(const std::vector<char>& code) {
		VkShaderModuleCreateInfo info{};

		info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		info.codeSize = code.size();
		info.pCode = reinterpret_cast<const ui32*>(code.data());

		VkShaderModule shaderModule;
		if (vkCreateShaderModule(vkData->Device.GetLogical(), &info, nullptr, &shaderModule) != VK_SUCCESS)
			throw std::runtime_error("Failed to create Vulkan shader module!");

		return shaderModule;
	}

	/* ---------------- */

	void VulkanRenderer::CreateRenderPass() {
		VkAttachmentDescription colorAttachment{};

		colorAttachment.format = vkData->Swapchain.GetFormat();
		colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;

		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;

		colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		VkAttachmentDescription depthAttachment{};

		depthAttachment.format = vkData->DepthTexture.FindDepthFormat();
		depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;

		depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

		depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

		depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		VkAttachmentReference colorRef{};

		colorRef.attachment = 0;
		colorRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkAttachmentReference depthRef{};

		depthRef.attachment = 1;
		depthRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpass{};

		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colorRef;

		subpass.pDepthStencilAttachment = &depthRef;

		VkSubpassDependency dependency{};

		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;

		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
		dependency.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

		std::array<VkAttachmentDescription, 2> attachments = { colorAttachment, depthAttachment };

		VkRenderPassCreateInfo renderInfo{};

		renderInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;

		renderInfo.attachmentCount = static_cast<ui32>(attachments.size());
		renderInfo.pAttachments = attachments.data();

		renderInfo.subpassCount = 1;
		renderInfo.pSubpasses = &subpass;

		renderInfo.dependencyCount = 1;
		renderInfo.pDependencies = &dependency;

		if (vkCreateRenderPass(vkData->Device.GetLogical(), &renderInfo, nullptr, &m_RenderPass) != VK_SUCCESS)
			throw std::runtime_error("Failed to create Vulkan render pass!");
	}

	void VulkanRenderer::CreateGraphicsPipeline() {
		auto vertShaderCode = ReadFile("assets/shaders/vert.spv");
		auto fragShaderCode = ReadFile("assets/shaders/frag.spv");

		VkShaderModule vertShaderModule = CreateShaderModule(vertShaderCode);
		VkShaderModule fragShaderModule = CreateShaderModule(fragShaderCode);

		VkPipelineShaderStageCreateInfo vertInfo{};

		vertInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		vertInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;

		vertInfo.module = vertShaderModule;
		vertInfo.pName = "main";
		vertInfo.pSpecializationInfo = nullptr;

		VkPipelineShaderStageCreateInfo fragInfo{};

		fragInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		fragInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;

		fragInfo.module = fragShaderModule;
		fragInfo.pName = "main";
		fragInfo.pSpecializationInfo = nullptr;

		VkPipelineShaderStageCreateInfo shaderStages[] = { vertInfo, fragInfo };

		auto bindingDesc = Vertex::GetBindingDescription();
		auto attriDesc = Vertex::GetAttributeDescriptions();

		VkPipelineVertexInputStateCreateInfo vertInputInfo{};

		vertInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

		vertInputInfo.vertexBindingDescriptionCount = 1;
		vertInputInfo.pVertexBindingDescriptions = &bindingDesc;

		vertInputInfo.vertexAttributeDescriptionCount = static_cast<ui32>(attriDesc.size());
		vertInputInfo.pVertexAttributeDescriptions = attriDesc.data();

		VkPipelineInputAssemblyStateCreateInfo inputAssembly{};

		inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		
		inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		
		inputAssembly.primitiveRestartEnable = VK_FALSE;

		VkViewport viewport{};

		viewport.x = 0.0f;
		viewport.y = 0.0f;
		
		viewport.width = (f32)vkData->Swapchain.GetExtent().width;
		viewport.height = (f32)vkData->Swapchain.GetExtent().height;
		
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		VkRect2D scissor{};

		scissor.offset = { 0, 0 };
		scissor.extent = vkData->Swapchain.GetExtent();

		VkPipelineViewportStateCreateInfo viewportState{};

		viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		
		viewportState.viewportCount = 1;
		viewportState.scissorCount = 1;

		VkPipelineRasterizationStateCreateInfo rasterizer{};

		rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		
		rasterizer.depthClampEnable = VK_FALSE;
		
		rasterizer.rasterizerDiscardEnable = VK_FALSE;
		
		rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
		rasterizer.lineWidth = 1.0f;
		rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
		rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
		rasterizer.depthBiasEnable = VK_FALSE;

		VkPipelineMultisampleStateCreateInfo multisampling{};

		multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;

		multisampling.sampleShadingEnable = VK_FALSE;

		multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

		VkPipelineColorBlendAttachmentState colorBlendAttachment{};

		colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		
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

		VkPipelineDepthStencilStateCreateInfo depthStencil{};

		depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;

		depthStencil.depthTestEnable = VK_TRUE;
		depthStencil.depthWriteEnable = VK_TRUE;

		depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;

		depthStencil.depthBoundsTestEnable = VK_FALSE;
		depthStencil.minDepthBounds = 0.0f;		// Optional
		depthStencil.maxDepthBounds = 1.0f;		// Optional

		depthStencil.stencilTestEnable = VK_FALSE;
		depthStencil.front = {};				// Optional
		depthStencil.back = {};					// Optional

		std::vector<VkDynamicState> dynamicStates = {
			VK_DYNAMIC_STATE_VIEWPORT,
			VK_DYNAMIC_STATE_SCISSOR
		};

		VkPipelineDynamicStateCreateInfo dynamicState{};

		dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;

		dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
		dynamicState.pDynamicStates = dynamicStates.data();

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};

		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

		pipelineLayoutInfo.setLayoutCount = 1;
		pipelineLayoutInfo.pSetLayouts = vkData->Descriptor.GetLayout();

		pipelineLayoutInfo.pushConstantRangeCount = 0;

		if (vkCreatePipelineLayout(vkData->Device.GetLogical(), &pipelineLayoutInfo, nullptr, &m_PipelineLayout) != VK_SUCCESS)
			throw std::runtime_error("failed to create pipeline layout!");

		VkGraphicsPipelineCreateInfo pipelineInfo{};

		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;

		pipelineInfo.stageCount = 2;
		pipelineInfo.pStages = shaderStages;

		pipelineInfo.pVertexInputState = &vertInputInfo;
		pipelineInfo.pInputAssemblyState = &inputAssembly;
		pipelineInfo.pViewportState = &viewportState;
		pipelineInfo.pRasterizationState = &rasterizer;
		pipelineInfo.pMultisampleState = &multisampling;
		pipelineInfo.pDepthStencilState = &depthStencil;
		pipelineInfo.pColorBlendState = &colorBlending;
		pipelineInfo.pDynamicState = &dynamicState;

		pipelineInfo.layout = m_PipelineLayout;

		pipelineInfo.renderPass = m_RenderPass;
		pipelineInfo.subpass = 0;

		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
		pipelineInfo.basePipelineIndex = -1; // Optional

		if (vkCreateGraphicsPipelines(vkData->Device.GetLogical(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_GraphicsPipeline) != VK_SUCCESS)
			throw std::runtime_error("Failed to create Vulkan graphics pipeline!");

		vkDestroyShaderModule(vkData->Device.GetLogical(), vertShaderModule, nullptr);
		vkDestroyShaderModule(vkData->Device.GetLogical(), fragShaderModule, nullptr);
	}

}	// Ocean
