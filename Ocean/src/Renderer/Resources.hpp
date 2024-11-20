#pragma once

#include "Ocean/Core/Primitives/Array.hpp"

// libs
#include <vulkan/vulkan.hpp>

#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/matrix_float4x4.hpp>

namespace Ocean {

	namespace Vulkan {

		#define CHECK_RESULT(result, message)   OASSERTM(result == VK_SUCCESS, message)



		struct Vertex {
			glm::vec2 pos;
			glm::vec3 color;

			/**
			 * @return The binding description of the vertex data for Vulkan.
			 */
			static inline VkVertexInputBindingDescription GetBindingDescription() {
				VkVertexInputBindingDescription description{ };

				// Specifies the index of the binding.
				description.binding = 0;
				// Sepcifies the number of bytes of one entry (aka one vertex).
				description.stride = sizeof(Vertex);
				/**
				 * VK_VERTEX_INPUT_RATE_VERTEX: Move to the next data entry after each vertex.
				 *
				 * VK_VERTEX_INPUT_RATE_INSTANCE: Move to the next data entry after each instance.
				 */
				description.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

				return description;
			}
			/**
			 * @return The attributes of each component of the Vertex as a fixed-size array.
			 */
			static inline FixedArray<VkVertexInputAttributeDescription> GetAttributeDescriptions() {
				FixedArray<VkVertexInputAttributeDescription> attributes(2);
				attributes.SetSize(2);

				// Specifies the index of the binding.
				attributes.Get(0).binding = 0;
				// Specifies the 'location' of the input for the vertex shader.
				attributes.Get(0).location = 0;
				/**
				 * Single Float: VK_FORMAT_R32_SFLOAT
				 *
				 * vec2 Float: VK_FORMAT_R32G32_SFLOAT
				 *
				 * vec3 Float: VK_FORMAT_R32G32B32_SFLOAT
				 *
				 * vec4 Float: VK_FORMAT_R32G32B32A32_SFLOAT
				 *
				 * There are other types defined to be able to be used as well.
				 */
				attributes.Get(0).format = VK_FORMAT_R32G32_SFLOAT;
				// Specifies the number of bytes since the beginning of the entry (aka beginning of the vertex).
				attributes.Get(0).offset = offsetof(Vertex, pos);

				attributes.Get(1).binding = 0;
				attributes.Get(1).location = 1;
				attributes.Get(1).format = VK_FORMAT_R32G32B32_SFLOAT;
				attributes.Get(1).offset = offsetof(Vertex, color);

				return attributes;
			}
		};

		struct UniformBufferObject {
			glm::mat4 model;
			glm::mat4 view;
			glm::mat4 proj;
		};



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

	#ifdef OC_DEBUG

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
				// oprint(CONSOLE_TEXT_GREEN("\nVulkan: %s\n\n"), pCallbackData->pMessage);
			}

			return VK_FALSE;
		}

		static VkResult CreateDebugUtilsMessengerEXT(
			VkInstance instance,
			const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
			const VkAllocationCallbacks* pAllocator,
			VkDebugUtilsMessengerEXT* pDebugMessenger
		) {
			auto func = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT"));
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
			auto func = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT"));
			if (func != nullptr) {
				func(instance, debugMessenger, pAllocator);
			}
		}
		
	#endif

	}	// Vulkan

}	// Ocean
