workspace "OceanEngine"
	architecture "x86_64"
	platforms "x64"
	startproject "Sandbox"

	configurations { "DebugLib", "DebugDLL", "ReleaseLib", "ReleaseDLL" }

	flags { "MultiProcessorCompile" }

	outputdir = "%{cfg.buildcfg}-%{cfg.system}"

	VULKAN_SDK = os.getenv("VULKAN_SDK")

	-- Include Directories Relative To Workspace Folder
	IncludeDir = { }
	IncludeDir["StackWalker"] = "%{wks.location}/Ocean/vendor/StackWalker/Main"
	IncludeDir["TLSF"       ] = "%{wks.location}/Ocean/vendor/tlsf"
	IncludeDir["GLFW"       ] = "%{wks.location}/Ocean/vendor/glfw/include"
	IncludeDir["IMGUI"      ] = "%{wks.location}/Ocean/vendor/imgui"
	IncludeDir["VulkanSDK"  ] = "%{VULKAN_SDK}/Include"
	IncludeDir["vkMemAlloc" ] = "%{wks.location}/Ocean/vendor/VulkanMemoryAllocator/include"

	LibraryDir = { }
	LibraryDir["VulkanSDK"  ] = "%{VULKAN_SDK}/Lib"

	Library = { }
	Library["Vulkan"        ] = "%{LibraryDir.VulkanSDK}/vulkan-1.lib"
	Library["VulkanUtils"   ] = "%{LibraryDir.VulkanSDK}/VkLayer_utils.lib"

	group "Dependencies"
	 	include "Ocean/vendor/premake5-GLFW.lua"
	 	include "Ocean/vendor/premake5-ImGui.lua"
	group ""

	group "Core"
		include "Ocean.lua"
	group ""

	group "Apps"
		include "Sandbox.lua"
	group ""

newaction {
	trigger = "clean",
	description = "Remove all non-source code files.",
	execute = function()
		print("Removing binaries")
		os.rmdir("./bin")
		
		print("Removing intermediate binaries")
		os.rmdir("./bin-int")

		print("Removing project files")
		os.rmdir("./.vs")
		os.remove("**.sln")
		os.remove("**.vcxproj")
		os.remove("**.vcxproj.filters")
		os.remove("**.vcxproj.user")

		print("Done")
	end
}
