#pragma once

#include "Ocean/Core/Primitives/Array.hpp"

#include "Renderer/Framebuffer.hpp"
#include "Renderer/Resources.hpp"

namespace Ocean {

	namespace Vulkan {

		class Renderer;
		class Device;



		/**
		 * @brief The configuration struct for the SwapChain class.
		 */
		struct SwapChainConfig {
			Allocator* allocator = nullptr;
			Renderer* renderer = nullptr;
			Device* device = nullptr;

			void* window = nullptr;

			SwapChainConfig(
				Allocator* allocator,
				Renderer* renderer,
				Device* device,
				void* window
			) : allocator(allocator), renderer(renderer), device(device), window(window) { }
		};

		/**
		 * @brief A struct of the Vulkan Image and Vulkan Image View to be stored by the swapchain for each frame in flight.
		 */
		struct SwapChainBuffer {
			VkImage Image = VK_NULL_HANDLE;
			VkImageView View = VK_NULL_HANDLE;
		};

		/**
		 * @brief Creates and manages VkSurfaceKHR, VkSwapchainKHR, and the according data such as the format and images / image views.
		 */
		class SwapChain {
		private:
			/**
			 * @brief A struct of the indices used for submitting commands.
			 */
			struct QueueFamilyIndices {
				u16 GraphicsIndex;
				u16 PresentationIndex;
			};

		public:
			/**
			 * @brief Initializes the SwapChain class and finds family queue indices, color formats, and creates the surface.
			 * @param config - Pointer to the config to use
			 */
			void Init(SwapChainConfig* config);
			void Shutdown();

			/**
			 * @brief Creates the swapchain and gets its images with the given width and height.
			 * @param width - Pointer to the width of the swapchain (may be adjusted by swapchain)
			 * @param height - Pointer to the height of the swapchain (may be adjusted by swapchain)
			 * @param vsync - (Optional, default = false) Can be used to force vsync rendering using VK_PRESENT_MODE_FIFO_KHR presentation mode
			 * @param fullscreen - N/A
			 */
			void CreateSwapChain(u16* width, u16* height, b8 vsync = false, b8 fullscreen = false);
			void RecreateSwapChain(u16* width, u16* height, b8 vsync = false, b8 fullscreen = false);

			/**
			 * @brief Acquires the next image in the swapchain.
			 * @param presentComplete - Semaphore that is signaled when the image is ready for use
			 * @param imageIndex - Pointer to the image index that will be adjusted to the next acquired image
			 * @return VkResult of the image acquisition.
			 */
			VkResult GetNextImage(VkSemaphore presentComplete, u32* imageIndex);

			/**
			 * @brief Queue an image for presentation.
			 * @param queue - The presentation queue for the image
			 * @param imageIndex - Index of the image to queue for presentation
			 * @param waitSemaphore - (Optional, default = VK_NULL_HANDLE) Sempahore that is waited on before the image is presented
			 * @return VkResult of the queue presentation.
			 */
			VkResult QueuePresentation(VkQueue queue, u32 imageIndex, VkSemaphore waitSemaphore = VK_NULL_HANDLE);

			/**
			 * @brief Tells the SwapChain to create the Vulkan Framebuffers when the rest of the renderer is ready.
			 */
			void CreateFramebuffers();

			/**
			 * @param index - The index of the framebuffer (in terms of max frames of flight).
			 * @return The Vulkan Framebuffer.
			 */
			VkFramebuffer GetFramebuffer(u32 index) const;

			/**
			 * @return The Vulkan Format for the swapchain color space.
			 */
			VkFormat GetColorFormat() const { return m_ColorFormat; }
			/**
			 * @return The Vulkan Extent that the swapchian is sized for (ie width, height, and origin).
			 */
			VkExtent2D GetExtent() const { return m_Extent; }

			/**
			 * @return Gets the Graphics Queue Index being used to submit commands.
			 */
			u16 GetGraphicsIndex() const { return m_QueueNodeIndices.GraphicsIndex; }
			/**
			 * @return Gets the Presentation Queue Index being used to submit commands.
			 */
			u16 GetPresentationIndex() const { return m_QueueNodeIndices.PresentationIndex; }

			/**
			 * @return The total number of Vulkan Image Views that are being used.
			 */
			u32 GetViewCount() const { return m_ImageCount; }
			/**
			 * @param index - The index of the image view being requested.
			 * @return The Vulkan image View at the given index.
			 */
			VkImageView GetImageView(u32 index) const { return m_Buffers.Get(index)->View; }

		private:
			Allocator* p_Allocator;
			Renderer* p_Renderer;
			Device* p_Device;

			VkFormat m_ColorFormat;
			VkColorSpaceKHR m_ColorSpace;
			VkSwapchainKHR m_SwapChain;
			VkExtent2D m_Extent;

			QueueFamilyIndices m_QueueNodeIndices;

			u32 m_ImageCount;

			FixedArray<VkImage> m_Images;
			FixedArray<SwapChainBuffer> m_Buffers;
			FixedArray<Framebuffer> m_Framebuffers;

		};

	}	// Vulkan

}	// Ocean
