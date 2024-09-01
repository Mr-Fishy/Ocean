project "ImGui"
	location "ImGui"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"
	warnings "off"

	targetdir ("ImGui/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("ImGui/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"ImGui/imconfig.h",
		"ImGui/imgui.h",
		"ImGui/imgui.cpp",
		"ImGui/imgui_draw.cpp",
		"ImGui/imgui_internal.h",
		"ImGui/imgui_tables.cpp",
		"ImGui/imgui_widgets.cpp",
		"ImGui/imstb_rectpack.h",
		"ImGui/imstb_textedit.h",
		"ImGui/imstb_truetype.h",
		"ImGui/imgui_demo.cpp"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"