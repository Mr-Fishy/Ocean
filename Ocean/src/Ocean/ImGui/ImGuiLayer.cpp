
#include "ocpch.hpp"
#include "ImGuiLayer.hpp"

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "Ocean/Core/Application.hpp"

// TEMPORARY
#include <GLFW/glfw3.h>
#include <glad/gl.h>

namespace Ocean {

	ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer") {}

	void ImGuiLayer::OnAttach() {
		OC_PROFILE_FUNCTION();

		// Setup Dear ImGui Context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;			// Enable Keyboard Controls
		// io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;			// Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;				// Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;				// Enable Multi-Viewport / Platform Windows
		// io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleFonts;		// Scale Fonts Based On DPI
		// io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleViewports;	// Scale Fonts Based On DPI
		// io.ConfigFlags |= ImGuiConfigFlags_IsSRGB;					// Enable S-RGB Color

		float windowDPI = 1.0f; // TODO: Get DPI Here
		io.Fonts->AddFontFromFileTTF("assets/fonts/NotoSans/static/NotoSans-Bold.ttf", 18.0f * windowDPI);
		io.Fonts->AddFontFromFileTTF("assets/fonts/OpenSans/static/OpenSans-SemiBold.ttf", 18.0f * windowDPI);

		// Setup Dear ImGui Style
		ImGui::StyleColorsDark();
		// ImGui::StyleColorsClassic();

		// When Viewports Are Enabled We Tweak WindowRounding / WindowBg So Platform Windows Can Look Identical To Regular Ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			style.WindowRounding = 6.0f;  // 6.0f Matches Windows 11 Rounding But ImGui Viewports Do Not Have Rounding Outside Of Application Window
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		SetDarkThemeColors();

		style.FramePadding = ImVec2{ 2.0f, 2.0f };
		style.ItemSpacing = ImVec2{ 8.0f, 6.0f };

		style.WindowBorderSize = 0.0f;
		style.ChildBorderSize = 0.0f;
		style.PopupBorderSize = 0.0f;
		style.TabBorderSize = 0.0f;
		style.TabBarBorderSize = 2.0f;
		style.FrameBorderSize = 0.0f;

		style.ChildRounding = 6.0f;
		style.PopupRounding = 2.0f;
		style.TabRounding = 2.0f;
		style.FrameRounding = 2.0f;
		style.ScrollbarRounding = 2.0f;

		style.AntiAliasedFill = true;
		style.AntiAliasedLines = true;
		style.CurveTessellationTol = 1.0f;

		style.WindowMinSize = ImVec2{ 370.0f, 200.0f };

		style.WindowMenuButtonPosition = ImGuiDir_Right;

		Application& app = Application::Get();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

		// Setup Platform / Renderer Bindings
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 460");
	}

	void ImGuiLayer::SetDarkThemeColors() {
		// Colors are 0->1 scale, so each color is formatted as RGBA: { R / 255, G / 255, B / 255, A (1) }
		//
		auto& colors = ImGui::GetStyle().Colors;
		// Text			{ 222, 222, 222, 255 }
		//
		ImVec4 text = { 222.0f / 255, 222.0f / 255, 222.0f / 255, 1.0f };
		// Background	{ 46, 53, 61, 255 }
		//
		ImVec4 background = { 46.0f / 255, 53.0f / 255, 61.0f / 255, 1.0f };
		// Primary		{ 36, 55, 112, 102 }
		//
		ImVec4 primary = { 36.0f / 255, 55.0f / 255, 112.0f / 255, 0.4f };
		// Secondary	{ 81, 67, 91, 102 }
		//
		ImVec4 secondary = { 81.0f / 255, 67.0f / 255, 91.0f / 255, 0.4f };
		// Accent		{ 154, 201, 249, 102 }
		//
		ImVec4 accent = { 154.0f / 255, 201.0f / 255, 249.0f / 255, 0.4f };

		colors[ImGuiCol_WindowBg]			= background;
		// colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		// colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
		// colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
		// colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);

		// Window Modal Tint
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

		colors[ImGuiCol_Text]				= text;
		// colors[ImGuiCol_TextDisabled]           = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);

		// Docking
		colors[ImGuiCol_DockingEmptyBg]		= background;
		colors[ImGuiCol_DockingPreview]		= accent;

		// Title
		colors[ImGuiCol_TitleBg] = primary;
		colors[ImGuiCol_TitleBgActive] = primary;
		colors[ImGuiCol_TitleBgCollapsed] = primary;

		// Navigation
		// colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		// colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
		// colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);

		// Tabs
		colors[ImGuiCol_Tab] = primary;
		colors[ImGuiCol_TabHovered] = secondary;
		colors[ImGuiCol_TabActive] = accent;
		colors[ImGuiCol_TabUnfocused] = primary;
		colors[ImGuiCol_TabUnfocusedActive] = secondary;

		// Headers
		colors[ImGuiCol_Header] = primary;
		colors[ImGuiCol_HeaderHovered] = secondary;
		colors[ImGuiCol_HeaderActive] = accent;

		// Seperator
		// colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
		// colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
		// colors[ImGuiCol_SeparatorActive] = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);

		// Resize Handle
		// colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.20f);
		// colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
		// colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);

		// Frame BG
		colors[ImGuiCol_FrameBg] = secondary;
		colors[ImGuiCol_FrameBgHovered] = primary;
		colors[ImGuiCol_FrameBgActive] = accent;

		// MenuBar
		// colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);

		// Scrollbar
		// colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
		// colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
		// colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
		// colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);

		// Buttons
		colors[ImGuiCol_Button]				= secondary;
		colors[ImGuiCol_ButtonHovered]		= primary;
		colors[ImGuiCol_ButtonActive]		= accent;

		// Checkmark
		// colors[ImGuiCol_CheckMark] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);

		// Selected Text
		// colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);

		// Slider
		// colors[ImGuiCol_SliderGrab] = ImVec4(0.24f, 0.52f, 0.88f, 1.00f);
		// colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);

		// Plots
		// colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		// colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
		// colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		// colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);

		// Tables
		colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
		colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
		colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
		colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);

		// Drag / Drop
		colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	}

	void ImGuiLayer::OnDetach() {
		OC_PROFILE_FUNCTION();

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::OnEvent(Event& e) {
		if (m_BlockEvents) {
			ImGuiIO& io = ImGui::GetIO();

			e.Handled |= e.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
			e.Handled |= e.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
		}
	}

	void ImGuiLayer::Begin() {
		OC_PROFILE_FUNCTION();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiLayer::End() {
		OC_PROFILE_FUNCTION();

		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		
		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

		// Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			GLFWwindow* backup_current_context = glfwGetCurrentContext();

			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();

			glfwMakeContextCurrent(backup_current_context);
		}
	}

}	// Ocean
