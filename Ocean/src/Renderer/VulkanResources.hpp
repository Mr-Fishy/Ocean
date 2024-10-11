#pragma once

#include "Ocean/Core/Types/ValueTypes.hpp"
#include "Ocean/Core/Primitives/Assert.hpp"

// TODO: Convert to use the vulkan memory allocator.
#pragma warning(push, 0)
#pragma warning(disable : 26495) // Uninitialized Warning
#include <vulkan/vulkan.hpp>
// #include "vk_mem_alloc.h"
#pragma warning(pop)

namespace Ocean {

	namespace Vulkan {

		#define CHECK_RESULT(result, message)   OASSERTM(result == VK_SUCCESS, message)



		inline const u32 k_InvalidIndex = 0xffffffff;

		typedef u32 ResourceHandle;



		inline void BeginCommandBuffer(VkCommandBuffer buffer) {
			VkCommandBufferBeginInfo info{ };
			info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

			info.flags = 0;
			info.pInheritanceInfo = nullptr;

			CHECK_RESULT(
				vkBeginCommandBuffer(buffer, &info),
				"Failed to begin recording command buffer!"
			);
		}

		inline void EndCommandBuffer(VkCommandBuffer buffer) {
			CHECK_RESULT(
				vkEndCommandBuffer(buffer),
				"Failed to record command buffer!"
			);
		}

		struct RenderPassInfo {
			VkCommandBuffer Buffer;
			VkRenderPass RenderPass;
			VkFramebuffer Framebuffer;
			VkExtent2D Extent;

			RenderPassInfo(
				VkCommandBuffer buffer,
				VkRenderPass renderPass,
				VkFramebuffer framebuffer,
				VkExtent2D extent
			) : Buffer(buffer), RenderPass(renderPass), Framebuffer(framebuffer), Extent(extent) { }
		};

		inline void BeginRenderPass(const RenderPassInfo& passInfo) {
			VkRenderPassBeginInfo info{ };
			info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;

			info.renderPass = passInfo.RenderPass;
			info.framebuffer = passInfo.Framebuffer;

			info.renderArea.offset = { 0, 0 };
			info.renderArea.extent = passInfo.Extent;

			VkClearValue color = { { { 0.3f, 0.3f, 0.3f, 1.0f } } };
			info.clearValueCount = 1;
			info.pClearValues = &color;

			vkCmdBeginRenderPass(passInfo.Buffer, &info, VK_SUBPASS_CONTENTS_INLINE);
		}

		inline void EndRenderPass(VkCommandBuffer buffer) {
			vkCmdEndRenderPass(buffer);
		}



		/* Not Yet Needed
		VkBool32 GetSupportedDepthFormat(VkPhysicalDevice physical, VkFormat* depthFormat);

		VkBool32 GetSupportedDepthStencilFormat(VkPhysicalDevice physical, VkFormat* depthStencilFormat);



		VkBool32 FormatIsFilterable(VkPhysicalDevice physical, VkFormat format, VkImageTiling tiling);

		VkBool32 FormatHasStencil(VkFormat format);



		void SetImageLayout();

		void SetImageLayout();

		void InsertImageMemoryBarrier();



		cstring ErrorCodeToString(VkResult code);

		cstring DeviceTypeToString(VkPhysicalDeviceType type);
		*/

		/**
		 * @brief Sets the Vulkan debug callback for validation layer messages.
		 * @param messageSeverity - The severity of the message. 
		 * @details VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT = 0x00000001,
		 * @details VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT = 0x00000010,
		 * @details VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT = 0x00000100,
		 * @details VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT = 0x00001000,
		 * @details VK_DEBUG_UTILS_MESSAGE_SEVERITY_FLAG_BITS_MAX_ENUM_EXT = 0x7FFFFFFF
		 * 
		 * @param messageType - The type of debug message.
		 * @details VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT = 0x00000001,
		 * @details VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT = 0x00000002,
		 * @details VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT = 0x00000004,
		 * @details VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT = 0x00000008,
		 * @details VK_DEBUG_UTILS_MESSAGE_TYPE_FLAG_BITS_MAX_ENUM_EXT = 0x7FFFFFFF
		 * 
		 * @param pCallbackData - The Vulkan callback data of the error.
		 * @param pUserData - The User callback data of the error.
		 * @return VK_SUCCESS if successfull error report, flagged otherwise.
		 */
		static VKAPI_ATTR VkBool32 VKAPI_CALL oDebugCallback(
			VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType,
			const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
			void* pUserData
		) {
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

		static VkResult CreateDebugUtilsMessengerEXT(
			VkInstance instance,
			const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
			const VkAllocationCallbacks* pAllocator,
			VkDebugUtilsMessengerEXT* pDebugMessenger
		) {
			auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
			if (func != nullptr) {
				return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
			}
			else {
				return VK_ERROR_EXTENSION_NOT_PRESENT;
			}
		}

		static void DestroyDebugUtilsMessengerEXT(
			VkInstance instance,
			VkDebugUtilsMessengerEXT debugMessenger,
			const VkAllocationCallbacks* pAllocator
		) {
			auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
			if (func != nullptr) {
				func(instance, debugMessenger, pAllocator);
			}
		}

	}	// Vulkan

}	// Ocean
