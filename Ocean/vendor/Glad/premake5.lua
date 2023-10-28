project "Glad"
    kind "StaticLib"
    language "C"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "include/vk_platform.h",
        "include/gl/gl.h",
        "include/gl/vulkan.h",
        "include/KHR/khrplatform.h",

        "src/gl.c",
        "src/vulkan.c",
    }

	includedirs
	{
		"include"
	}

    filter "system:windows"
        systemversion "latest"
        staticruntime "On"

    filter { "system:windows", "configurations:Release" }
        buildoptions "/MT"