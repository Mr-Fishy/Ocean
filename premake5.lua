workspace "OceanEngine"
	architecture "x64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Dist",
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include Directories Relative To Root Folder (Solution Directory)
IncludeDir = {}
IncludeDir["GLFW"] = "Ocean/vendor/GLFW/include"
IncludeDir["Glad"] = "Ocean/vendor/Glad/include"
IncludeDir["ImGui"] = "Ocean/vendor/imgui"
IncludeDir["glm"] = "Ocean/vendor/glm"
IncludeDir["stb_image"] = "Ocean/vendor/stb_image"

group "Vendor"
	include "Ocean/vendor/GLFW"
	include "Ocean/vendor/Glad"
	include "Ocean/vendor/imgui"
group ""

project "Ocean"
	location "Ocean"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "ocpch.hpp"
	pchsource "Ocean/src/ocpch.cpp"

	defines
	{
		"GLM_FORCE_CTOR_INIT",
		"_CRT_SECURE_NO_WARNINGS",
		"_SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING",
	}

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib",
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"OC_PLATFORM_WINDOWS",
			"OC_BUILD_DLL",
			"GLFW_INCLUDE_NONE",
		}
	
	filter "configurations:Debug"
		defines"OC_DEBUG"
		runtime "Debug"
		symbols"on"

	filter "configurations:Release"
		defines "OC_RELEASE"
		runtime "Release"
		optimize "on"
		
	filter "configurations:Dist"
		defines "OC_DIST"
		runtime "Release"
		optimize "on"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/src/**.cpp",
	}

	includedirs
	{
		"Ocean/vendor/spdlog/include",
		"Ocean/src",
		"Ocean/vendor",
		"%{IncludeDir.glm}",
	}

	links
	{
		"Ocean",
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"OC_PLATFORM_WINDOWS",
		}

	filter "configurations:Debug"
		defines"OC_DEBUG"
		runtime "Debug"
		symbols"on"

	filter "configurations:Release"
		defines "OC_RELEASE"
		runtime "Release"
		optimize "on"
		
	filter "configurations:Dist"
		defines "OC_DIST"
		runtime "Release"
		optimize "on"
