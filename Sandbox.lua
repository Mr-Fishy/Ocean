project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir    ("bin-int/" .. outputdir .. "/%{prj.name}")

	defines {
		"_CRT_SECURE_NO_WARNINGS"
	}

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/src/**.cpp",
	}

	includedirs {
		"%{wks.location}/Ocean/src",
		"%{wks.location}/Ocean/vendor",
		
		"%{IncludeDir.VulkanSDK}",
		"%{IncludeDir.vkMemAlloc}",
	}

	links {
		"Ocean"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:DebugLib"
		defines "OC_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:DebugDLL"
		defines "OC_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:ReleaseLib"
		defines "OC_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:ReleaseDLL"
		defines "OC_RELEASE"
		runtime "Release"
		optimize "on"