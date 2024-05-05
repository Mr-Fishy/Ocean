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
		"_SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING",	-- Silence std::iterator Depracation Warnings (C++20)
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