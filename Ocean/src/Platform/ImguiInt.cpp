#include "ImguiInt.hpp"

#include <imgui.h>
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

namespace Ocean {

	static ImguiService s_ImguiService;

	ImguiService* ImguiService::Instance() {
		return &s_ImguiService;
	}

	void ImguiService::Init(void* config) {
		ImGuiServiceConfig* imguiConfig = (ImGuiServiceConfig*)config;
		m_GPU = imguiConfig->GPU;

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGuiIO& io = ImGui::GetIO();
		(void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;			// Enable Keyboard Controls
		// io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;			// Enable Gamepad Controls
		// io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleFonts;			// Scale Fonts Based On DPI
		// io.ConfigFlags |= ImGuiConfigFlags_IsSRGB;					// Enable S-RGB Color

		// ImGui_ImplVulkan_InitInfo info{ };

		ImGui_ImplGlfw_InitForVulkan((GLFWwindow*)imguiConfig->WindowHandle, true);
		ImGui_ImplOpenGL3_Init("#version 460");
	}

	void ImguiService::Shutdown() {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();

		ImGui::DestroyContext();
	}

	void ImguiService::NewFrame() {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();

		ImGui::NewFrame();
	}

	void ImguiService::Render() {
		ImGui::Render();

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void ImguiService::RemoveCachedTexture(TextureHandle& texture) {
	}

	void ImguiService::SetStyle(ImGuiStyles style) {
	}

}	// Ocean
