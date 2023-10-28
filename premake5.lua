workspace "OceanEngine"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release",
        "Dist",
    }

    startproject "Sandbox"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include Directories Relative To Root Folder (Solution Directory)
IncludeDir = {}
IncludeDir["GLFW"] = "Ocean/vendor/GLFW/include"
IncludeDir["Glad"] = "Ocean/vendor/Glad/include"
IncludeDir["ImGui"] = "Ocean/vendor/imgui"

include "Ocean/vendor/GLFW"
include "Ocean/vendor/Glad"
include "Ocean/vendor/imgui"

project "Ocean"
    location "Ocean"
    kind "SharedLib"
    language "C++"
    staticruntime "Off"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "ocpch.hpp"
    pchsource "Ocean/src/ocpch.cpp"

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.hpp",
        "%{prj.name}/src/**.cpp",
    }

    includedirs
    {
        "%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}",
    }

    links
    {
        "GLFW",
        "Glad",
        "ImGui",
        "opengl32.lib",
        "dwmapi.lib",
    }

    filter "system:windows"
        cppdialect "C++20"
        systemversion "latest"

        defines
        {
            "OC_PLATFORM_WINDOWS",
            "OC_BUILD_DLL",
            "GLFW_INCLUDE_NONE",
        }

        postbuildcommands
        {
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
        }
    
    filter "configurations:Debug"
        defines"OC_DEBUG"
        runtime "Debug"
        symbols"On"

    filter "configurations:Release"
        defines "OC_RELEASE"
        runtime "Release"
        optimize "On"
        
    filter "configurations:Dist"
        defines "OC_DIST"
        runtime "Release"
        optimize "On"

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    staticruntime "Off"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

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
    }

    links
    {
        "Ocean",
    }

    filter "system:windows"
        cppdialect "C++20"
        systemversion "latest"

        defines
        {
            "OC_PLATFORM_WINDOWS",
        }

    filter "configurations:Debug"
        defines"OC_DEBUG"
        runtime "Debug"
        symbols"On"

    filter "configurations:Release"
        defines "OC_RELEASE"
        runtime "Release"
        optimize "On"
        
    filter "configurations:Dist"
        defines "OC_DIST"
        runtime "Release"
        optimize "On"