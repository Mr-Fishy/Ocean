#include "ocpch.hpp"

#include "Vk/VulkanData.hpp"

namespace Ocean {

	void VulkanData::Init() {
		Instance.Init();
		Device.Init();

		Swapchain.Init();


		Renderer.Init();

		Buffer.InitPool();

		DepthTexture.Init();

		Framebuffer.Init();

		// Texture Init
		StaticTexture = new VulkanTexture;
		Sampler.Init();

		VertexBuffer.Init();
		IndexBuffer.Init();
		UniformBuffer.Init();

		Descriptor.InitPool();

		Buffer.InitBuffers();

		Sync.Init();
	}

	void VulkanData::Destroy() {
		Swapchain.CleanSwapchain();

		Renderer.Destroy();

		UniformBuffer.Destroy();

		Descriptor.DestroyPool();

		Sampler.Destroy();
		delete StaticTexture;

		Descriptor.DestroyLayout();

		IndexBuffer.Destroy();
		VertexBuffer.Destroy();

		Sync.Destroy();

		Buffer.Destroy();

		Device.Destroy();
		Instance.Destroy();
	}

	VulkanData::VulkanData(ContextSpecs spec) :
		Instance(spec.AppName), MaxFramesInFlight(spec.MaxFramesInFlight), Window(spec.Window), StaticTexture(nullptr),
		GraphicsQueue(VK_NULL_HANDLE), PresentQueue(VK_NULL_HANDLE), Surface(VK_NULL_HANDLE) { }

	VulkanData::~VulkanData() = default;

}	// Ocean
