#pragma once

#include "Ocean/Core/Types/ValueTypes.hpp"
#include "Ocean/Core/Primitives/Assert.hpp"

#include <vulkan/vulkan.hpp>
// #include "vk_mem_alloc.h"

namespace Ocean {

	namespace Vulkan {

		inline const u32 k_InvalidIndex = 0xffffffff;

		typedef u32 ResourceHandle;



		struct BufferHandle { ResourceHandle Index; };

		struct TextureHandle { ResourceHandle Index; };

		struct ShaderStateHandle { ResourceHandle Index; };

		struct SamplerHandle { ResourceHandle Index; };

		struct DescriptorSetLayoutHandle { ResourceHandle Index; };

		struct DescriptorSetHandle { ResourceHandle Index; };

		struct PipelineHandle { ResourceHandle Index; };

		struct RenderPassHandle { ResourceHandle Index; };



		inline const u8 k_MaxImageOutputs         = 8;
		inline const u8 k_MaxDescriptorSetLayouts = 8;
		inline const u8 k_MaxShaderStages         = 5;
		inline const u8 k_MaxDescriptorsPerSet    = 16;
		inline const u8 k_MaxVertexStreams        = 16;
		inline const u8 k_MaxVertexAttributes     = 16;

		inline const u32 k_SubmitHeaderSentinel   = 0xfefeb7ba;
		inline const u32 k_MaxResourceDeletions   = 64;



		struct Rect2d {
			f32 x = 0.0f;
			f32 y = 0.0f;

			f32 width = 0.0f;
			f32 height = 0.0f;

		};	// Rect2d

		struct Rect2dInt {
			i16 x = 0;
			i16 y = 0;

			u16 width = 0;
			u16 height = 0;

		};	// Rect2dInt

		struct Viewport {
			Rect2dInt view;

			f32 minDepth = 0.0f;
			f32 maxDepth = 0.0f;

		};	// Viewport



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
			if (result != VK_SUCCESS)
				OASSERTM(false, CONSOLE_TEXT_RED("\n%s | Result: %i\n"), message, result);
		}

	}	// Vulkan

}	// Ocean
