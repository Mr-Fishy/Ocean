project "Glad"
	location "glad"
	kind "StaticLib"
	language "C"
	staticruntime "off"
	warnings "off"

	targetdir ("./bin/" .. outputdir .. "/%{prj.name}")
	objdir ("./bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"Glad/include/vk_platform.h",
		"Glad/include/gl/gl.h",
		"Glad/include/gl/vulkan.h",
		"Glad/include/KHR/khrplatform.h",

		"Glad/src/gl.c",
		"Glad/src/vulkan.c",
	}

	includedirs
	{
		"Glad/include"
	}

	filter "system:windows"
		systemversion "latest"
		
	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
