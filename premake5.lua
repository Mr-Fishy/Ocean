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
IncludeDir["ENTT"]			= "%{wks.location}/Ocean/vendor/ENTT/single_include"
IncludeDir["Glad"]			= "%{wks.location}/Ocean/vendor/Glad/include"
IncludeDir["GLFW"]			= "%{wks.location}/Ocean/vendor/GLFW/include"
IncludeDir["glm"]			= "%{wks.location}/Ocean/vendor/glm"
IncludeDir["ImGui"]			= "%{wks.location}/Ocean/vendor/ImGui"
IncludeDir["ImGuizmo"]		= "%{wks.location}/Ocean/vendor/ImGuizmo"
IncludeDir["stb_image"]		= "%{wks.location}/Ocean/vendor/stb_image"
IncludeDir["yaml_cpp"]		= "%{wks.location}/Ocean/vendor/yaml-cpp/include"

group "Dependencies"
	include "Ocean/vendor/premake5-GLFW.lua"
	include "Ocean/vendor/premake5-Glad.lua"
	include "Ocean/vendor/premake5-ImGui.lua"
	include "Ocean/vendor/premake5-yaml-cpp.lua"
group ""

include "Ocean.lua"
include "Coral.lua"
include "Sandbox.lua"
