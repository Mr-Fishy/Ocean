#pragma once

#include "Ocean/Core/Primitives/Service.hpp"
#include "Ocean/Core/Types/Strings.hpp"

namespace Ocean {

	class Device;

	class CommandBuffer;

	class TextureHandle;



	struct ImDrawData;

	enum ImGuiStyles {
		Default = 0,
		GreenBlue,
		DarkRed,
		DarkGold
	};	// ImGuiStyles

	struct ImGuiServiceConfig {

		Device* GPU;

		void* WindowHandle;

	};	// ImGuiServiceConfig


	class ImguiService : public Ocean::Service {
	public:
		OCEAN_DECLARE_SERVICE(ImguiService);

		void Init(void* config) override;
		void Shutdown() override;

		void NewFrame();
		void Render();

		static cstring Name() { return "OCEAN_Imgui_Service"; }

	private:
		void RemoveCachedTexture(TextureHandle& texture);

		void SetStyle(ImGuiStyles style);

		/* --- */

		Device* m_GPU;
	};

}	// Ocean
