workspace "OceanEngine"
	architecture "x86_64"
	startproject "Coral"

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
IncludeDir["EnTT"] = "Ocean/vendor/EnTT/include"

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
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE",
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
		"%{IncludeDir.EnTT}",
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

	defines
	{
		"_SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING"		-- Silence std::iterator Depracation Warnings (C++20)
	}

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

project "Coral"
	location "Coral"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	defines
	{
		"_SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING"		-- Silence std::iterator Depracation Warnings (C++20)
	}

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
		"%{IncludeDir.EnTT}",
	}

	links
	{
		"Ocean",
	}

	filter "system:windows"
		systemversion "latest"

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
