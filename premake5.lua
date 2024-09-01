workspace "OceanEngine"
	architecture "x86_64"
	platforms "x64"
	startproject "Sandbox"

	configurations { "Debug", "Release" }

	flags { "MultiProcessorCompile" }

	outputdir = "%{cfg.buildcfg}-%{cfg.system}"

	-- Include Directories Relative To Workspace Folder
	IncludeDir = {}
	IncludeDir["StackWalker"] = "%{wks.location}/Ocean/vendor/StackWalker/Main"
	IncludeDir["TLSF"       ] = "%{wks.location}/Ocean/vendor/tlsf"
	IncludeDir["GLFW"       ] = "%{wks.location}/Ocean/vendor/glfw/include"
	IncludeDir["IMGUI"      ] = "%{wks.location}/Ocean/vendor/imgui"
	IncludeDir["vkMemAlloc" ] = "%{wks.location}/Ocean/vendor/VulkanMemoryAllocator/include"

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
