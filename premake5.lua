require "premake-cmake/cmake"

workspace "OceanEngine"
	architecture "x64"
	platforms "x64"
	startproject "Sandbox"

	configurations { "Debug", "Release" }

	flags { "MultiProcessorCompile" }

	outputdir = "%{cfg.buildcfg}-%{cfg.system}"

	-- Include Directories Relative To Workspace Folder
	IncludeDir = {}
	IncludeDir["GLFW"] = "%{wks.location}/Ocean/vendor/glfw/include"
	IncludeDir["GLM"]  = "%{wks.location}/Ocean/vendor/glm"

	group "Dependencies"
		include "Ocean/vendor/premake5-GLFW.lua"
	group ""

	include "Ocean.lua"
	include "Sandbox.lua"
	include "Coral.lua"

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

		os.remove("**.cmake")
		os.remove("./CMakeLists.txt")

		os.remove("**Makefile")
        print("Done")
	end
}
