project "Ocean"
	location "Ocean"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir    ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "ocpch.hpp"
	pchsource "%{prj.name}/src/ocpch.cpp"

	defines {
		"_CRT_SECURE_NO_WARNINGS",
		-- "GLFW_INCLUDE_NONE"
	}

	files {
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/src/**.cpp",

		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",
	}

	includedirs {
		"%{prj.name}/src",
		"%{prj.name}/src/Ocean",
		
		"$(VULKAN_SDK)/Include",

		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLAD}",
		"%{IncludeDir.GLM}",
		"%{IncludeDir.STB_IMAGE}",
		"%{IncludeDir.VMA}",
	}

	links {
		"$(VULKAN_SDK)/Lib/vulkan-1.lib",
		
		"GLFW",
		"Glad",
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
