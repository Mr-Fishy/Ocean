project "imgui"
	location "imgui"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"
	warnings "off"

	targetdir ("./bin/" .. outputdir .. "/%{prj.name}")
	objdir ("./bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/imconfig.h",
		"%{prj.name}/imgui.h",
		"%{prj.name}/imgui.cpp",
		"%{prj.name}/imgui_draw.cpp",
		"%{prj.name}/imgui_internal.h",
		"%{prj.name}/imgui_tables.cpp",
		"%{prj.name}/imgui_widgets.cpp",
		"%{prj.name}/imstb_rectpack.h",
		"%{prj.name}/imstb_textedit.h",
		"%{prj.name}/imstb_truetype.h",
		"%{prj.name}/imgui_demo.cpp",
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"