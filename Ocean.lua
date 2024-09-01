project "Ocean"
	location "Ocean"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

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

		"%{IncludeDir.vkMemAlloc}/vk_mem_alloc.h"
	}

	includedirs {
		"%{prj.name}/src",
		"%{prj.name}/src/Ocean",
		
		"$(VULKAN_SDK)/Include",

		"%{IncludeDir.StackWalker}",
		"%{IncludeDir.TLSF}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.IMGUI}",
		"%{IncludeDir.vkMemAlloc}",
	}

	links {
		"$(VULKAN_SDK)/Lib/vulkan-1.lib",

		"GLFW",
		"ImGui",
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "OC_DEBUG"
		runtime "Debug"
		symbols "on"

		links {
			-- "$(VULKAN_SDK)/Lib/shaderc_sharedd.lib",
			"$(VULKAN_SDK)/Lib/spirv-cross-cored.lib",
			"$(VULKAN_SDK)/Lib/spirv-cross-glsld.lib",
		}

	filter "configurations:Release"
		defines "OC_RELEASE"
		runtime "Release"
		optimize "on"

		links {
			-- "$(VULKAN_SDK)/Lib/shaderc_shared.lib",
			"$(VULKAN_SDK)/Lib/spirv-cross-core.lib",
			"$(VULKAN_SDK)/Lib/spirv-cross-glsl.lib",
		}
