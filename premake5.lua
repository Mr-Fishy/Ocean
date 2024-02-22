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
-- Does not like '-' in names, use '_'
IncludeDir = {}
IncludeDir["CTTI"]			= "%{wks.location}/Ocean/vendor/CTTI/include"
IncludeDir["ENTT"]			= "%{wks.location}/Ocean/vendor/ENTT/single_include"
IncludeDir["Glad"]			= "%{wks.location}/Ocean/vendor/Glad/include"
IncludeDir["GLFW"]			= "%{wks.location}/Ocean/vendor/GLFW/include"
IncludeDir["glm"]			= "%{wks.location}/Ocean/vendor/glm"
IncludeDir["ImGui"]			= "%{wks.location}/Ocean/vendor/ImGui"
IncludeDir["ImGuizmo"]		= "%{wks.location}/Ocean/vendor/ImGuizmo"
IncludeDir["MemAllocators"] = "%{wks.location}/Ocean/vendor/MemAllocators/includes"
IncludeDir["stb_image"]		= "%{wks.location}/Ocean/vendor/stb_image"
IncludeDir["yaml_cpp"]		= "%{wks.location}/Ocean/vendor/yaml-cpp/include"

group "Dependencies"
	include "Ocean/vendor/premake5-GLFW.lua"
	include "Ocean/vendor/premake5-Glad.lua"
	include "Ocean/vendor/premake5-ImGui.lua"
	include "Ocean/vendor/premake5-yaml-cpp.lua"
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
		"_SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING",	-- Silence std::iterator Depracation Warnings (C++20)
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE",
		"YAML_CPP_STATIC_DEFINE",
	}

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/src/**.cpp",

		"%{prj.name}/vendor/ImGuizmo/ImGuizmo.h",
		"%{prj.name}/vendor/ImGuizmo/ImGuizmo.cpp",

		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",

		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		-- "%{IncludeDir.CTTI}",
		"%{IncludeDir.ENTT}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.ImGuizmo}",
		-- "%{IncludeDir.MemAllocators}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.yaml_cpp}",
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib",
		"yaml-cpp",
	}

	filter "files:Ocean/vendor/ImGuizmo/**.cpp"
		flags { "NoPCH" }

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
		-- "%{IncludeDir.CTTI}",
		"%{IncludeDir.ENTT}",
		"%{IncludeDir.glm}",
		-- "%{IncludeDir.MemAllocators}",
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
		-- "%{IncludeDir.CTTI}",
		"%{IncludeDir.ENTT}",
		"%{IncludeDir.glm}",
		-- "%{IncludeDir.MemAllocators}",
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
