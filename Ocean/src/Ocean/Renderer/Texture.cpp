#include "ocpch.hpp"

#include "Ocean/Renderer/Texture.hpp"

#include "Ocean/Renderer/Renderer.hpp"
#include "Platform/OpenGL/OpenGLTexture.hpp"
#include "Platform/Vk/VulkanTexture.hpp"

namespace Ocean {

	Ref<Texture2D> Texture2D::Create(ui32 width, ui32 height) {
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None:
				return nullptr;

			case RendererAPI::API::OpenGL:
				return CreateRef<GL::OpenGLTexture2D>(width, height);

			case RendererAPI::API::Vulkan:
				return CreateRef<Vk::VulkanTexture2D>(width, height);
		}

		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(const std::string& path) {
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None:
				return nullptr;

			case RendererAPI::API::OpenGL:
				return CreateRef<GL::OpenGLTexture2D>(path);

			case RendererAPI::API::Vulkan:
				return CreateRef<Vk::VulkanTexture2D>(path);
		}

		return nullptr;
	}

}	// Ocean
