workspace "OceanEngine"
	architecture "x64"
	startproject "Testbox"

	configurations { "Debug", "Release", "Dist" }

	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

	-- Include Directories Relative To Workspace Folder
	-- IncludeDir = {}

	-- Include Dependencies

	include "Ocean.lua"
	include "Testbox.lua"
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

		os.remove("**Makefile")
        print("Done")
	end
}
