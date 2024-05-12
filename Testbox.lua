project "Testbox"
	location "Testbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir    ("bin-int/" .. outputdir .. "/%{prj.name}")

	defines {
		"_CRT_SECURE_NO_WARNINGS"
	}

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.c",
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/src/**.cpp",
	}

	includedirs {
		"Ocean/src",
	}

	links {
		"Ocean"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "TB_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "TB_RELEASE"
		runtime "Release"
		optimize "on"
	
	filter "configurations:Dist"
		defines "TB_DIST"
		runtime "Release"
		optimize "on"
