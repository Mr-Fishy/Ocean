#pragma once

// Ocean
#include <Ocean/Renderer/Texture.hpp>

// std
#include <filesystem>

namespace Ocean {

	class ContentBrowserPanel {
	public:
		ContentBrowserPanel();

		void OnImGuiRender();

	private:
		std::filesystem::path m_CurrentDirectory;

		// TODO: Dynamic Icon List, Loaded From Folder On Initialization
		Ref<Texture2D> m_DirectoryIcon;
		Ref<Texture2D> m_FileIcon;
	};

}	// Ocean
