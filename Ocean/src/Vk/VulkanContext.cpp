#include "ocpch.hpp"

#include "VulkanContext.hpp"

#include "Ocean/Logger/Log.hpp"

// std
#include <map>
#include <set>
#include <fstream>

#include <glm/gtc/matrix_transform.hpp>

namespace Ocean {

	static VkResult CreateDebugUtilsMessengerEXT(
		VkInstance instance,
		const VkDebugUtilsMessengerCreateInfoEXT* pInfo,
		const VkAllocationCallbacks* pAllocator,
		VkDebugUtilsMessengerEXT* pMessenger) {

		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
		if (func != nullptr)
			return func(instance, pInfo, pAllocator, pMessenger);

		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}

	static void DestroyDebugUtilsMessengerEXT(VkInstance instance,
		VkDebugUtilsMessengerEXT debugMessenger,
		const VkAllocationCallbacks* pAllocator) {
	
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
		if (func != nullptr) {
			func(instance, debugMessenger, pAllocator);
		}
	}

	static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData) {

		std::stringstream ss;

		ss << "Vulkan |: " << pCallbackData->pMessage;

		switch (messageSeverity) {
			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
				OC_CORE_TRACE(ss.str());
				break;
			
			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
				OC_CORE_INFO(ss.str());
				break;
			
			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
				OC_CORE_WARN(ss.str());
				break;
			
			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
				OC_CORE_ERROR(ss.str());
				break;
		}

		return VK_FALSE;
	}

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



	VulkanContext::VulkanContext(GLFWwindow* window) : m_Window(window), m_Data(nullptr) {
		m_Data = new VulkanData();

		CreateInstance();

		InitDebugMessenger();

		CreateSurface();

		PickPhysicalDevice();

		CreateLogicalDevice();

		CreateSwapChain();

		CreateImageViews();

		CreateRenderPass();

		CreateDescriptorSetLayout();

		CreateGraphicsPipeline();

		CreateFrameBuffers();

		CreateCommandPool();

		CreateVertexBuffer();

		CreateIndexBuffer();

		CreateUniformBuffers();

		CreateDescriptorPool();

		CreateDescriptorSets();

		CreateCommandBuffers();

		CreateSyncObjects();
	}

	VulkanContext::~VulkanContext() {
		vkDestroyPipeline(m_Data->Device, m_GraphicsPipeline, nullptr);
		vkDestroyPipelineLayout(m_Data->Device, m_PipelineLayout, nullptr);
		vkDestroyRenderPass(m_Data->Device, m_RenderPass, nullptr);

		CleanSwapChain();

		for (ui8 i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
			vkDestroyBuffer(m_Data->Device, m_Data->UniformBuffers[i], nullptr);
			vkFreeMemory(m_Data->Device, m_Data->UniformBuffersMemory[i], nullptr);
		}

		vkDestroyDescriptorPool(m_Data->Device, m_Data->DescriptorPool, nullptr);
		vkDestroyDescriptorSetLayout(m_Data->Device, m_DescriptorSetLayout, nullptr);

		vkDestroyBuffer(m_Data->Device, m_Data->IndexBuffer, nullptr);
		vkFreeMemory(m_Data->Device, m_Data->IndexBufferMemory, nullptr);

		vkDestroyBuffer(m_Data->Device, m_Data->VertexBuffer, nullptr);
		vkFreeMemory(m_Data->Device, m_Data->VertexBufferMemory, nullptr);

		for (ui8 i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
			vkDestroySemaphore(m_Data->Device, v_RenderFinsishedSemaphores[i], nullptr);
			vkDestroySemaphore(m_Data->Device, v_ImageAvailableSemaphores[i], nullptr);
			vkDestroyFence(m_Data->Device, v_InFlightFences[i], nullptr);
		}

		vkDestroyCommandPool(m_Data->Device, m_CommandPool, nullptr);

		vkDestroyDevice(m_Data->Device, nullptr);

		#ifdef OC_DEBUG
		DestroyDebugUtilsMessengerEXT(m_Data->Instance, m_Messenger, nullptr);
		#endif // OC_DEBUG

		vkDestroySurfaceKHR(m_Data->Instance, m_Data->Surface, nullptr);
		vkDestroyInstance(m_Data->Instance, nullptr);

		delete m_Data;
	}

	void VulkanContext::DrawFrame() {
		vkWaitForFences(m_Data->Device, 1, &v_InFlightFences[m_CurrentFrame], VK_TRUE, UINT64_MAX);

		ui32 imageIndex;
		VkResult result;
		result = vkAcquireNextImageKHR(m_Data->Device, m_SwapChain, UINT64_MAX, v_ImageAvailableSemaphores[m_CurrentFrame], VK_NULL_HANDLE, &imageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR) {
			RecreateSwapChain();
			return;
		}
		else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
			throw std::runtime_error("Failed to acquire Vulkan swap chain image!");

		vkResetFences(m_Data->Device, 1, &v_InFlightFences[m_CurrentFrame]);

		vkResetCommandBuffer(v_CommandBuffers[m_CurrentFrame], /* VkCommandBufferResetFlags */ 0);
		RecordCommandBuffer(v_CommandBuffers[m_CurrentFrame], imageIndex);

		UpdateUniformBuffer(m_CurrentFrame);

		VkSubmitInfo info{};

		info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore waitSemaphores[] = { v_ImageAvailableSemaphores[m_CurrentFrame] };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

		info.waitSemaphoreCount = 1;
		info.pWaitSemaphores = waitSemaphores;
		info.pWaitDstStageMask = waitStages;

		info.commandBufferCount = 1;
		info.pCommandBuffers = &v_CommandBuffers[m_CurrentFrame];

		VkSemaphore signalSemaphores[] = { v_RenderFinsishedSemaphores[m_CurrentFrame] };

		info.signalSemaphoreCount = 1;
		info.pSignalSemaphores = signalSemaphores;

		if (vkQueueSubmit(m_Data->GraphicsQueue, 1, &info, v_InFlightFences[m_CurrentFrame]) != VK_SUCCESS)
			throw std::runtime_error("Failed to submit Vulkan draw command buffer!");

		VkPresentInfoKHR presentInfo{};

		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;

		VkSwapchainKHR swapChains[] = { m_SwapChain };

		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;

		presentInfo.pImageIndices = &imageIndex;

		presentInfo.pResults = nullptr; // Optional

		result = vkQueuePresentKHR(m_Data->PresentQueue, &presentInfo);

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_FramebufferResized) {
			m_FramebufferResized = false;
			RecreateSwapChain();
		}
		else if (result != VK_SUCCESS)
			throw std::runtime_error("Failed to present Vulkan swap chain image!");

		m_CurrentFrame = (m_CurrentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
	}

	void VulkanContext::CreateInstance() {
		#ifdef OC_DEBUG
		if (!CheckValidationLayerSupport())
			throw std::runtime_error("Validation layers requested, but not available!");
		#endif // OC_DEBUG

		VkApplicationInfo appInfo{};

		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Ocean App";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "Ocean Engine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;

		VkInstanceCreateInfo createInfo{};

		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;

		auto extensions = GetRequiredExtensions();
		createInfo.enabledExtensionCount = static_cast<ui32>(extensions.size());
		createInfo.ppEnabledExtensionNames = extensions.data();

		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
		#ifdef OC_DEBUG
		createInfo.enabledLayerCount = static_cast<ui32>(c_ValidationLayers.size());
		createInfo.ppEnabledLayerNames = c_ValidationLayers.data();

		SetDebugMessengerCreateInfo(debugCreateInfo);
		createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
		#elif
		createInfo.enabledLayerCount = 0;
		createInfo.pNext = nullptr;
		#endif // OC_DEBUG

		if (vkCreateInstance(&createInfo, nullptr, &m_Data->Instance) != VK_SUCCESS)
			throw std::runtime_error("Failed to create Vulkan instance!");
	}

	b8 VulkanContext::CheckValidationLayerSupport() {
		ui32 layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		std::set<std::string> requiredLayers(c_ValidationLayers.begin(), c_ValidationLayers.end());

		for (const auto& layer : c_ValidationLayers)
			requiredLayers.erase(layer);

		return requiredLayers.empty();
	}

	std::vector<const char*> VulkanContext::GetRequiredExtensions() {
		ui32 glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

		#ifdef OC_DEBUG
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		#endif // OC_DEBUG

		return extensions;
	}

	void VulkanContext::InitDebugMessenger() {
		VkDebugUtilsMessengerCreateInfoEXT createInfo;
		SetDebugMessengerCreateInfo(createInfo);

		if (CreateDebugUtilsMessengerEXT(m_Data->Instance, &createInfo, nullptr, &m_Messenger) != VK_SUCCESS)
			throw std::runtime_error("Failed to set up Vulkan debug messenger!");
	}

	void VulkanContext::SetDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& info) {
		info = {};
		
		info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
							 | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
							 | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
						 | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
						 | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		info.pfnUserCallback = DebugCallback;
	}

	void VulkanContext::CreateSurface() {
		if (glfwCreateWindowSurface(m_Data->Instance, m_Window, nullptr, &m_Data->Surface) != VK_SUCCESS)
			throw std::runtime_error("Failed to create Vulkan window surface!");
	}

	void VulkanContext::PickPhysicalDevice() {
		ui32 deviceCount = 0;
		vkEnumeratePhysicalDevices(m_Data->Instance, &deviceCount, nullptr);

		if (deviceCount == 0)
			throw std::runtime_error("Failed to find GPUs with Vulkan support!");

		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(m_Data->Instance, &deviceCount, devices.data());

		std::multimap<ui8, VkPhysicalDevice> candidates;

		for (const auto& device : devices) {
			candidates.insert(std::make_pair(RateDeviceSuitability(device), device));
		}

		if (candidates.rbegin()->first > 0)
			m_Data->PhysicalDevice = candidates.rbegin()->second;
		else
			throw std::runtime_error("Failed to find a suitable GPU!");
	}

	b8 VulkanContext::IsDeviceSuitable(VkPhysicalDevice device) {
		QueueFamilyIndices indices = FindQueueFamilies(device);

		b8 extensionsSupported = CheckDeviceExtensionSupport(device);

		b8 swapChainAdequate = false;
		if (extensionsSupported) {
			SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(device);
			swapChainAdequate = !swapChainSupport.Formats.empty() && !swapChainSupport.PresentModes.empty();
		}

		return indices.IsComplete() && extensionsSupported && swapChainAdequate;
	}

	b8 VulkanContext::CheckDeviceExtensionSupport(VkPhysicalDevice device) {
		ui32 extensionCount = 0;
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

		std::set<std::string> requiredExtensions(c_DeviceExtensions.begin(), c_DeviceExtensions.end());

		for (const auto& extension : availableExtensions)
			requiredExtensions.erase(extension.extensionName);

		return requiredExtensions.empty();
	}

	ui8 VulkanContext::RateDeviceSuitability(VkPhysicalDevice device) {
		ui8 score = 0;

		VkPhysicalDeviceProperties deviceProperties;
		vkGetPhysicalDeviceProperties(device, &deviceProperties);

		VkPhysicalDeviceFeatures deviceFeatures;
		vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

		if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
			score += 10;

		score += deviceProperties.limits.maxImageDimension2D % 100;

		if (!deviceFeatures.geometryShader)
			return 0;

		#ifdef OC_DEBUG
		std::stringstream ss;

		ss << "Vulkan |: Phsyical Device : " << deviceProperties.deviceName;
		
		OC_CORE_INFO(ss.str());
		#endif // OC_DEBUG
		return score;
	}

	VulkanContext::QueueFamilyIndices VulkanContext::FindQueueFamilies(VkPhysicalDevice device) {
		QueueFamilyIndices indices;
		
		ui32 queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

		i32 i = 0;
		for (const auto& queueFamily : queueFamilies) {
			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
				indices.GraphicsFamily = i;

			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(m_Data->PhysicalDevice, i, m_Data->Surface, &presentSupport);
			if (presentSupport)
				indices.PresentFamily = i;

			if (indices.IsComplete())
				break;

			i++;
		}

		return indices;
	}

	void VulkanContext::CreateLogicalDevice() {
		QueueFamilyIndices indices = FindQueueFamilies(m_Data->PhysicalDevice);

		std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
		std::set<ui32> uniqueQueueFamilies = { indices.GraphicsFamily.value(), indices.PresentFamily.value() };

		f32 queuePriority = 1.0f;
		for (ui32 queueFamily : uniqueQueueFamilies) {
			VkDeviceQueueCreateInfo queueCreateInfo{};

			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueCount = 1;
			queueCreateInfo.queueFamilyIndex = queueFamily;
			queueCreateInfo.pQueuePriorities = &queuePriority;

			queueCreateInfos.push_back(queueCreateInfo);
		}

		VkPhysicalDeviceFeatures deviceFeatures{};

		VkDeviceCreateInfo createInfo{};

		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

		createInfo.queueCreateInfoCount = static_cast<ui32>(queueCreateInfos.size());
		createInfo.pQueueCreateInfos = queueCreateInfos.data();

		createInfo.pEnabledFeatures = &deviceFeatures;

		createInfo.enabledExtensionCount = static_cast<ui32>(c_DeviceExtensions.size());
		createInfo.ppEnabledExtensionNames = c_DeviceExtensions.data();
		#ifdef OC_DEBUG
		createInfo.enabledLayerCount = static_cast<ui32>(c_ValidationLayers.size());
		createInfo.ppEnabledLayerNames = c_ValidationLayers.data();
		#elif
		createInfo.enabledLayerCount = 0;
		#endif // OC_DEBUG

		if (vkCreateDevice(m_Data->PhysicalDevice, &createInfo, nullptr, &m_Data->Device))
			throw std::runtime_error("Failed to create logical device!");

		vkGetDeviceQueue(m_Data->Device, indices.GraphicsFamily.value(), 0, &m_Data->GraphicsQueue);
		vkGetDeviceQueue(m_Data->Device, indices.PresentFamily.value(), 0, &m_Data->PresentQueue);
	}

	void VulkanContext::CreateSwapChain() {
		SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(m_Data->PhysicalDevice);

		VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.Formats);
		VkPresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.PresentModes);
		VkExtent2D extent = ChooseSwapExtent(swapChainSupport.Capabilities);

		ui32 imageCount = swapChainSupport.Capabilities.minImageCount + 1;
		if (swapChainSupport.Capabilities.maxImageCount > 0 && imageCount > swapChainSupport.Capabilities.maxImageCount)
			imageCount = swapChainSupport.Capabilities.maxImageCount;

		VkSwapchainCreateInfoKHR swapInfo{};

		swapInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swapInfo.surface = m_Data->Surface;

		swapInfo.minImageCount = imageCount;
		swapInfo.imageFormat = surfaceFormat.format;
		swapInfo.imageColorSpace = surfaceFormat.colorSpace;
		swapInfo.imageExtent = extent;
		swapInfo.imageArrayLayers = 1; // Always 1 unless rendering for stereoscopic video
		swapInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		QueueFamilyIndices indices = FindQueueFamilies(m_Data->PhysicalDevice);
		ui32 queueFamilyIndices[] = { indices.GraphicsFamily.value(), indices.PresentFamily.value() };

		if (indices.GraphicsFamily != indices.PresentFamily) {
			swapInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			swapInfo.queueFamilyIndexCount = 2;
			swapInfo.pQueueFamilyIndices = queueFamilyIndices;
		}
		else {
			swapInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			swapInfo.queueFamilyIndexCount = 0;			// Optional
			swapInfo.pQueueFamilyIndices = nullptr;		// Optional
		}

		swapInfo.preTransform = swapChainSupport.Capabilities.currentTransform;
		swapInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

		swapInfo.presentMode = presentMode;
		swapInfo.clipped = VK_TRUE;

		swapInfo.oldSwapchain = VK_NULL_HANDLE;

		if (vkCreateSwapchainKHR(m_Data->Device, &swapInfo, nullptr, &m_SwapChain) != VK_SUCCESS)
			throw std::runtime_error("Failed to create Vulkan swap chain!");

		vkGetSwapchainImagesKHR(m_Data->Device, m_SwapChain, &imageCount, nullptr);
		v_SwapChainImages.resize(imageCount);
		vkGetSwapchainImagesKHR(m_Data->Device, m_SwapChain, &imageCount, v_SwapChainImages.data());

		m_SwapChainImageFormat = surfaceFormat.format;
		m_SwapChainExtent = extent;
	}

	VulkanContext::SwapChainSupportDetails VulkanContext::QuerySwapChainSupport(VkPhysicalDevice device) {
		SwapChainSupportDetails details;

		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_Data->PhysicalDevice, m_Data->Surface, &details.Capabilities);

		ui32 formatCount = 0;
		vkGetPhysicalDeviceSurfaceFormatsKHR(m_Data->PhysicalDevice, m_Data->Surface, &formatCount, nullptr);
		if (formatCount != 0) {
			details.Formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(m_Data->PhysicalDevice, m_Data->Surface, &formatCount, details.Formats.data());
		}

		ui32 presentModeCount = 0;
		vkGetPhysicalDeviceSurfacePresentModesKHR(m_Data->PhysicalDevice, m_Data->Surface, &presentModeCount, nullptr);
		if (presentModeCount != 0) {
			details.PresentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(m_Data->PhysicalDevice, m_Data->Surface, &presentModeCount, details.PresentModes.data());
		}

		return details;
	}

	VkSurfaceFormatKHR VulkanContext::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
		for (const auto& format : availableFormats) {
			if (format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
				return format;
		}

		return availableFormats[0];
	}

	VkPresentModeKHR VulkanContext::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
		for (const auto& presentMode : availablePresentModes) {
			if (presentMode == VK_PRESENT_MODE_MAILBOX_KHR)
				return presentMode;
		}

		return VK_PRESENT_MODE_FIFO_KHR;
	}

	VkExtent2D VulkanContext::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
		if (capabilities.currentExtent.width != UINT32_MAX) {
			return capabilities.currentExtent;
		}
		
		i32 width, height;
		glfwGetFramebufferSize(m_Window, &width, &height);

		VkExtent2D actualExtent = {
			static_cast<ui32>(width),
			static_cast<ui32>(height)
		};

		actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
		actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

		return actualExtent;
	}

	void VulkanContext::CreateImageViews() {
		v_SwapChainImageViews.resize(v_SwapChainImages.size());

		for (ui64 i = 0; i < v_SwapChainImages.size(); i++) {
			VkImageViewCreateInfo viewInfo{};

			viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			viewInfo.image = v_SwapChainImages[i];

			viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			viewInfo.format = m_SwapChainImageFormat;

			viewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			viewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			viewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			viewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

			viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			viewInfo.subresourceRange.baseMipLevel = 0;
			viewInfo.subresourceRange.levelCount = 1;
			viewInfo.subresourceRange.baseArrayLayer = 0;
			viewInfo.subresourceRange.layerCount = 1;
		
			if (vkCreateImageView(m_Data->Device, &viewInfo, nullptr, &v_SwapChainImageViews[i]) != VK_SUCCESS)
				throw std::runtime_error("Failed to create Vulkan image views!");
		}
	}

	void VulkanContext::CreateRenderPass() {
		VkAttachmentDescription colorAttachment{};

		colorAttachment.format = m_SwapChainImageFormat;
		colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;

		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;

		colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		VkAttachmentReference colorRef{};

		colorRef.attachment = 0;
		colorRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpass{};

		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colorRef;

		VkSubpassDependency dependency{};
		
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;

		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.srcAccessMask = 0;

		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		VkRenderPassCreateInfo renderInfo{};

		renderInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;

		renderInfo.attachmentCount = 1;
		renderInfo.pAttachments = &colorAttachment;

		renderInfo.subpassCount = 1;
		renderInfo.pSubpasses = &subpass;

		renderInfo.dependencyCount = 1;
		renderInfo.pDependencies = &dependency;

		if (vkCreateRenderPass(m_Data->Device, &renderInfo, nullptr, &m_RenderPass) != VK_SUCCESS)
			throw std::runtime_error("Failed to create Vulkan render pass!");
	}

	void VulkanContext::CreateDescriptorSetLayout() {
		VkDescriptorSetLayoutBinding uboLayout{};

		uboLayout.binding = 0;
		uboLayout.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		uboLayout.descriptorCount = 1;

		uboLayout.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

		uboLayout.pImmutableSamplers = nullptr; // Optional

		VkDescriptorSetLayoutCreateInfo info{};

		info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;

		info.bindingCount = 1;
		info.pBindings = &uboLayout;

		if (vkCreateDescriptorSetLayout(m_Data->Device, &info, nullptr, &m_DescriptorSetLayout) != VK_SUCCESS)
			throw std::runtime_error("Failed to create Vulkan descriptor set layout!");
	}

	void VulkanContext::CreateGraphicsPipeline() {
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
		viewport.width = (f32)m_SwapChainExtent.width;
		viewport.height = (f32)m_SwapChainExtent.height;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		VkRect2D scissor{};
		
		scissor.offset = { 0, 0 };
		scissor.extent = m_SwapChainExtent;

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
		pipelineLayoutInfo.pSetLayouts = &m_DescriptorSetLayout;
		pipelineLayoutInfo.pushConstantRangeCount = 0;

		if (vkCreatePipelineLayout(m_Data->Device, &pipelineLayoutInfo, nullptr, &m_PipelineLayout) != VK_SUCCESS)
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
		pipelineInfo.pDepthStencilState = nullptr; // Optional
		pipelineInfo.pColorBlendState = &colorBlending;
		pipelineInfo.pDynamicState = &dynamicState;

		pipelineInfo.layout = m_PipelineLayout;

		pipelineInfo.renderPass = m_RenderPass;
		pipelineInfo.subpass = 0;

		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
		pipelineInfo.basePipelineIndex = -1; // Optional

		if (vkCreateGraphicsPipelines(m_Data->Device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_GraphicsPipeline) != VK_SUCCESS)
			throw std::runtime_error("Failed to create Vulkan graphics pipeline!");

		vkDestroyShaderModule(m_Data->Device, vertShaderModule, nullptr);
		vkDestroyShaderModule(m_Data->Device, fragShaderModule, nullptr);
	}

	VkShaderModule VulkanContext::CreateShaderModule(const std::vector<char>& code) {
		VkShaderModuleCreateInfo info{};

		info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		info.codeSize = code.size();
		info.pCode = reinterpret_cast<const ui32*>(code.data());

		VkShaderModule shaderModule;
		if (vkCreateShaderModule(m_Data->Device, &info, nullptr, &shaderModule) != VK_SUCCESS)
			throw std::runtime_error("Failed to create Vulkan shader module!");

		return shaderModule;
	}

	void VulkanContext::CreateFrameBuffers() {
		v_FrameBuffers.resize(v_SwapChainImageViews.size());

		for (size_t i = 0; i < v_SwapChainImageViews.size(); i++) {
			VkImageView attachments[] = { v_SwapChainImageViews[i] };

			VkFramebufferCreateInfo info{};

			info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			info.renderPass = m_RenderPass;
			info.attachmentCount = 1;
			info.pAttachments = attachments;
			info.width = m_SwapChainExtent.width;
			info.height = m_SwapChainExtent.height;
			info.layers = 1;

			if (vkCreateFramebuffer(m_Data->Device, &info, nullptr, &v_FrameBuffers[i]) != VK_SUCCESS)
				throw std::runtime_error("Failed to create Vulkan framebuffer!");
		}
	}

	void VulkanContext::CreateCommandPool() {
		QueueFamilyIndices queueFamilyIndices = FindQueueFamilies(m_Data->PhysicalDevice);

		VkCommandPoolCreateInfo info{};

		info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		info.queueFamilyIndex = queueFamilyIndices.GraphicsFamily.value();

		if (vkCreateCommandPool(m_Data->Device, &info, nullptr, &m_CommandPool) != VK_SUCCESS)
			throw std::runtime_error("Failed to create Vulkan command pool!");
	}

	void VulkanContext::CreateVertexBuffer() {
		VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();
		
		VkBuffer stagingBuffer;
		VkDeviceMemory  stagingMemory;
		CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingMemory);

		void* data;
		vkMapMemory(m_Data->Device, stagingMemory, 0, bufferSize, 0, &data);
			memcpy(data, vertices.data(), static_cast<ui64>(bufferSize));
		vkUnmapMemory(m_Data->Device, stagingMemory);

		CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_Data->VertexBuffer, m_Data->VertexBufferMemory);

		CopyBuffer(stagingBuffer, m_Data->VertexBuffer, bufferSize);

		vkDestroyBuffer(m_Data->Device, stagingBuffer, nullptr);
		vkFreeMemory(m_Data->Device, stagingMemory, nullptr);
	}

	void VulkanContext::CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags props, VkBuffer& buffer, VkDeviceMemory& mem) {
		VkBufferCreateInfo info{};

		info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;

		info.size = size;
		info.usage = usage;

		info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if (vkCreateBuffer(m_Data->Device, &info, nullptr, &buffer) != VK_SUCCESS)
			throw std::runtime_error("Failed to create Vulkan vertex buffer!");

		VkMemoryRequirements memReq;
		vkGetBufferMemoryRequirements(m_Data->Device, buffer, &memReq);

		VkMemoryAllocateInfo alloc{};

		alloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		alloc.allocationSize = memReq.size;
		alloc.memoryTypeIndex = FindMemoryType(memReq.memoryTypeBits, props);

		if (vkAllocateMemory(m_Data->Device, &alloc, nullptr, &mem) != VK_SUCCESS)
			throw std::runtime_error("Failed to allocate Vulkan vertex buffer memory!");

		vkBindBufferMemory(m_Data->Device, buffer, mem, 0);
	}

	ui32 VulkanContext::FindMemoryType(ui32 typeFilter, VkMemoryPropertyFlags properties) {
		VkPhysicalDeviceMemoryProperties mem;
		vkGetPhysicalDeviceMemoryProperties(m_Data->PhysicalDevice, &mem);

		for (ui32 i = 0; i < mem.memoryTypeCount; i++) {
			if (typeFilter & (1 << i) && (mem.memoryTypes[i].propertyFlags & properties) == properties)
				return i;
		}

		throw std::runtime_error("Failed to find suitable Vulkan memory type!");
	}

	void VulkanContext::CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
		VkCommandBufferAllocateInfo info{};

		info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		info.commandPool = m_CommandPool;
		info.commandBufferCount = 1;

		VkCommandBuffer commandBuffer;
		vkAllocateCommandBuffers(m_Data->Device, &info, &commandBuffer);

		{
			VkCommandBufferBeginInfo beginInfo{};

			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

			vkBeginCommandBuffer(commandBuffer, &beginInfo);

			VkBufferCopy copy{};

			copy.srcOffset = 0; // Optional
			copy.dstOffset = 0; // Optional
			copy.size = size;

			vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copy);

			vkEndCommandBuffer(commandBuffer);
		}

		VkSubmitInfo submit{};

		submit.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submit.commandBufferCount = 1;
		submit.pCommandBuffers = &commandBuffer;

		vkQueueSubmit(m_Data->GraphicsQueue, 1, &submit, VK_NULL_HANDLE);
		vkQueueWaitIdle(m_Data->GraphicsQueue);

		vkFreeCommandBuffers(m_Data->Device, m_CommandPool, 1, &commandBuffer);
	}

	void VulkanContext::CreateIndexBuffer() {
		VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingMemory;
		CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingMemory);

		void* data;
		vkMapMemory(m_Data->Device, stagingMemory, 0, bufferSize, 0, &data);
			memcpy(data, indices.data(), static_cast<ui64>(bufferSize));
		vkUnmapMemory(m_Data->Device, stagingMemory);

		CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_Data->IndexBuffer, m_Data->IndexBufferMemory);

		CopyBuffer(stagingBuffer, m_Data->IndexBuffer, bufferSize);

		vkDestroyBuffer(m_Data->Device, stagingBuffer, nullptr);
		vkFreeMemory(m_Data->Device, stagingMemory, nullptr);
	}

	void VulkanContext::CreateUniformBuffers() {
		VkDeviceSize bufferSize = sizeof(UniformBufferObject);

		m_Data->UniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
		m_Data->UniformBuffersMemory.resize(MAX_FRAMES_IN_FLIGHT);
		m_Data->UniformBuffersMapped.resize(MAX_FRAMES_IN_FLIGHT);

		for (ui8 i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
			CreateBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_Data->UniformBuffers[i], m_Data->UniformBuffersMemory[i]);
		
			vkMapMemory(m_Data->Device, m_Data->UniformBuffersMemory[i], 0, bufferSize, 0, &m_Data->UniformBuffersMapped[i]);
		}
	}

	void VulkanContext::UpdateUniformBuffer(ui32 currentImage) {
		static auto startTime = std::chrono::high_resolution_clock::now();

		auto currentTime = std::chrono::high_resolution_clock::now();
		f32 time = std::chrono::duration<f32, std::chrono::seconds::period>(currentTime - startTime).count();

		UniformBufferObject ubo{};

		ubo.model = glm::rotate(mat4(1.0f), time * glm::radians(90.0f), vec3(0.0f, 0.0f, 1.0f));
		ubo.view = glm::lookAt(vec3(2.0f, 2.0f, 2.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f));
		ubo.proj = glm::perspective(glm::radians(45.0f), (f32)m_SwapChainExtent.width / (f32)m_SwapChainExtent.height, 0.1f, 10.0f);

		ubo.proj[1][1] *= -1;

		memcpy(m_Data->UniformBuffersMapped[currentImage], &ubo, sizeof(ubo));
	}

	void VulkanContext::CreateDescriptorPool() {
		VkDescriptorPoolSize poolSize{};

		poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		poolSize.descriptorCount = static_cast<ui32>(MAX_FRAMES_IN_FLIGHT);

		VkDescriptorPoolCreateInfo info{};

		info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;

		info.poolSizeCount = 1;
		info.pPoolSizes = &poolSize;

		info.maxSets = static_cast<ui32>(MAX_FRAMES_IN_FLIGHT);

		if (vkCreateDescriptorPool(m_Data->Device, &info, nullptr, &m_Data->DescriptorPool) != VK_SUCCESS)
			throw std::runtime_error("Failed to create Vulkan descriptor pool!");
	}

	void VulkanContext::CreateDescriptorSets() {
		std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, m_DescriptorSetLayout);

		VkDescriptorSetAllocateInfo allocInfo{};

		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;

		allocInfo.descriptorPool = m_Data->DescriptorPool;
		allocInfo.descriptorSetCount = static_cast<ui32>(MAX_FRAMES_IN_FLIGHT);
		allocInfo.pSetLayouts = layouts.data();

		m_Data->DescriptorSets.resize(MAX_FRAMES_IN_FLIGHT);
		if (vkAllocateDescriptorSets(m_Data->Device, &allocInfo, m_Data->DescriptorSets.data()) != VK_SUCCESS)
			throw std::runtime_error("Failed to allocate Vulkan descriptor sets!");

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
			VkDescriptorBufferInfo bufferInfo{};
			bufferInfo.buffer = m_Data->UniformBuffers[i];
			bufferInfo.offset = 0;
			bufferInfo.range = sizeof(UniformBufferObject);

			VkWriteDescriptorSet descriptorWrite{};
			descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrite.dstSet = m_Data->DescriptorSets[i];
			descriptorWrite.dstBinding = 0;
			descriptorWrite.dstArrayElement = 0;
			descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			descriptorWrite.descriptorCount = 1;
			descriptorWrite.pBufferInfo = &bufferInfo;

			vkUpdateDescriptorSets(m_Data->Device, 1, &descriptorWrite, 0, nullptr);
		}
	}

	void VulkanContext::CreateCommandBuffers() {
		v_CommandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

		VkCommandBufferAllocateInfo info{};

		info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		info.commandPool = m_CommandPool;
		info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		info.commandBufferCount = static_cast<ui32>(v_CommandBuffers.size());

		if (vkAllocateCommandBuffers(m_Data->Device, &info, v_CommandBuffers.data()) != VK_SUCCESS)
			throw std::runtime_error("Failed to allocate Vulkan command buffers!");
	}

	void VulkanContext::CreateSyncObjects() {
		v_ImageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
		v_RenderFinsishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
		v_InFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

		VkSemaphoreCreateInfo semaInfo{};

		semaInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo fenceInfo{};

		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		for (ui8 i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
			if (vkCreateSemaphore(m_Data->Device, &semaInfo, nullptr, &v_ImageAvailableSemaphores[i]) != VK_SUCCESS
				|| vkCreateSemaphore(m_Data->Device, &semaInfo, nullptr, &v_RenderFinsishedSemaphores[i]) != VK_SUCCESS
				|| vkCreateFence(m_Data->Device, &fenceInfo, nullptr, &v_InFlightFences[i]) != VK_SUCCESS)
				throw std::runtime_error("Failed to create Vulkan semaphores!");
		}
	}

	void VulkanContext::RecordCommandBuffer(VkCommandBuffer commandBuffer, ui32 imageIndex) {
		VkCommandBufferBeginInfo info{};

		info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		info.flags = 0;						// Optional
		info.pInheritanceInfo = nullptr;	// Optional

		if (vkBeginCommandBuffer(commandBuffer, &info) != VK_SUCCESS)
			throw std::runtime_error("Failed to begin recording Vulkan command buffer!");

		VkRenderPassBeginInfo renderInfo{};

		renderInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderInfo.renderPass = m_RenderPass;
		renderInfo.framebuffer = v_FrameBuffers[imageIndex];

		renderInfo.renderArea.offset = { 0, 0 };
		renderInfo.renderArea.extent = m_SwapChainExtent;

		VkClearValue clearColor = { { {0.0f, 0.0f, 0.0f, 1.0f} } };

		renderInfo.clearValueCount = 1;
		renderInfo.pClearValues = &clearColor;

		vkCmdBeginRenderPass(commandBuffer, &renderInfo, VK_SUBPASS_CONTENTS_INLINE);
		
		{
			vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_GraphicsPipeline);

			VkViewport viewport{};

			viewport.x = 0.0f;
			viewport.y = 0.0f;
			viewport.width = static_cast<f32>(m_SwapChainExtent.width);
			viewport.height = static_cast<f32>(m_SwapChainExtent.height);
			viewport.minDepth = 0.0f;
			viewport.maxDepth = 1.0f;

			vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

			VkRect2D scissor{};

			scissor.offset = { 0, 0 };
			scissor.extent = m_SwapChainExtent;
			
			vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

			VkBuffer vertexBuffers[] = { m_Data->VertexBuffer };
			VkDeviceSize offsets[] = { 0 };
			vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
			
			vkCmdBindIndexBuffer(commandBuffer, m_Data->IndexBuffer, 0, VK_INDEX_TYPE_UINT16);

			vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_PipelineLayout, 0, 1, &m_Data->DescriptorSets[m_CurrentFrame], 0, nullptr);

			vkCmdDrawIndexed(commandBuffer, static_cast<ui32>(indices.size()), 1, 0, 0, 0);
		}
		
		vkCmdEndRenderPass(commandBuffer);

		if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
			throw std::runtime_error("Failed to record Vulkan command buffer!");
	}

	void VulkanContext::CleanSwapChain() {
		for (auto framebuffer : v_FrameBuffers)
			vkDestroyFramebuffer(m_Data->Device, framebuffer, nullptr);

		for (auto imageView : v_SwapChainImageViews)
			vkDestroyImageView(m_Data->Device, imageView, nullptr);

		vkDestroySwapchainKHR(m_Data->Device, m_SwapChain, nullptr);
	}

	void VulkanContext::RecreateSwapChain() {
		i32 width = 0, height = 0;
		glfwGetFramebufferSize(m_Window, &width, &height);

		while (width == 0 || height == 0) {
			glfwGetFramebufferSize(m_Window, &width, &height);
			glfwWaitEvents();
		}

		vkDeviceWaitIdle(m_Data->Device);

		CleanSwapChain();

		CreateSwapChain();
		CreateImageViews();
		CreateFrameBuffers();
	}

}
