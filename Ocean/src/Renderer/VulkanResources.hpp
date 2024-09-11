#pragma once

#include "Ocean/Core/Types/ValueTypes.hpp"
#include "Ocean/Core/Primitives/Assert.hpp"

#include <vulkan/vulkan.hpp>
// #include "vk_mem_alloc.h"

namespace Ocean {

	namespace Vulkan {

		inline const u32 k_InvalidIndex = 0xffffffff;

		typedef u32 ResourceHandle;



		VkBool32 GetSupportedDepthFormat(VkPhysicalDevice physical, VkFormat* depthFormat);

		VkBool32 GetSupportedDepthStencilFormat(VkPhysicalDevice physical, VkFormat* depthStencilFormat);



		VkBool32 FormatIsFilterable(VkPhysicalDevice physical, VkFormat format, VkImageTiling tiling);

		VkBool32 FormatHasStencil(VkFormat format);



		void SetImageLayout();

		void SetImageLayout();

		void InsertImageMemoryBarrier();



		cstring ErrorCodeToString(VkResult code);

		cstring DeviceTypeToString(VkPhysicalDeviceType type);

		static VKAPI_ATTR VkBool32 VKAPI_CALL oDebugCallback(
			VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType,
			const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
			if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
				OASSERTM(false, CONSOLE_TEXT_RED("\nVulkan: %s\n"), pCallbackData->pMessage);
			}
			else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
				oprint(CONSOLE_TEXT_YELLOW("\nVulkan: %s\n\n"), pCallbackData->pMessage);
			}
			else {
				oprint(CONSOLE_TEXT_GREEN("\nVulkan: %s\n\n"), pCallbackData->pMessage);
			}

			return VK_FALSE;
		}

		static VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
			auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
			if (func != nullptr) {
				return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
			}
			else {
				return VK_ERROR_EXTENSION_NOT_PRESENT;
			}
		}

		static void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
			auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
			if (func != nullptr) {
				func(instance, debugMessenger, pAllocator);
			}
		}

		static void CheckResultSuccess(VkResult result, cstring message) {
			OASSERTM(result == VK_SUCCESS, CONSOLE_TEXT_RED("\n%s | Result: %i\n"), message, result);
		}

	}	// Vulkan

}	// Ocean
