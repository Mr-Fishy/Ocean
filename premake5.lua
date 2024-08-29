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
	IncludeDir["TLSF"       ] = "%{wks.location}/Ocean/vendor/TLSF"
	IncludeDir["SDL3"       ] = "%{wks.location}/Ocean/vendor/SDL/include"
	IncludeDir["GLFW"       ] = "%{wks.location}/Ocean/vendor/GLFW/include"
	IncludeDir["vkMemAlloc" ] = "%{wks.location}/Ocean/vendor/VulkanMemoryAllocator/include"

	group "Dependencies"
	 	include "Ocean/vendor/premake5-GLFW.lua"
	--	include "Ocean/vendor/premake5-GLAD.lua"
	group ""

	include "Ocean.lua"
	include "Sandbox.lua"
	-- include "Coral.lua"

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
