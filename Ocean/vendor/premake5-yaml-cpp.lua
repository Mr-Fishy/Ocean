project "yaml-cpp"
	location "yaml-cpp"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("yaml-cpp/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("yaml-cpp/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"yaml-cpp/src/**.h",
		"yaml-cpp/src/**.cpp",
		
		"yaml-cpp/include/**.h"
	}

	includedirs
	{
		"yaml-cpp/include"
	}

	defines
	{
		"YAML_CPP_STATIC_DEFINE"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"