project "Ocean"
	location "Ocean"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	filter "*Lib"
		kind "StaticLib"

	filter "*DLL"
		kind "SharedLib"

	filter { }

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir    ("bin-int/" .. outputdir .. "/%{prj.name}")

	-- pchheader "ocpch.hpp"
	-- pchsource "%{prj.name}/src/ocpch.cpp"

	defines {
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE"
	}

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/src/**.cpp",

		"%{IncludeDir.StackWalker}/StackWalker/StackWalker.h",
		"%{IncludeDir.StackWalker}/StackWalker/StackWalker.cpp",

		"%{IncludeDir.TLSF}/tlsf.h",
		"%{IncludeDir.TLSF}/tlsf.c",

		"%{IncludeDir.vkMemAlloc}/vk_mem_alloc.h",

		"%{IncludeDir.IMGUI}/backends/imgui_impl_glfw.h",
		"%{IncludeDir.IMGUI}/backends/imgui_impl_glfw.cpp",

		"%{IncludeDir.IMGUI}/backends/imgui_impl_opengl3.h",
		"%{IncludeDir.IMGUI}/backends/imgui_impl_opengl3.cpp",
	}

	includedirs {
		"%{prj.name}/src",
		"%{prj.name}/src/Ocean",
		
		"%{IncludeDir.StackWalker}",
		"%{IncludeDir.TLSF}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.IMGUI}",
		"%{IncludeDir.VulkanSDK}",
		"%{IncludeDir.vkMemAlloc}",
	}

	links {
		"%{Library.Vulkan}",
		-- "%{Library.VulkanUtils}",

		"glfw",
		"imgui",
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
