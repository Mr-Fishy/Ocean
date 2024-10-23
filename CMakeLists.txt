cmake_minimum_required(VERSION 3.20)

project(OceanEngine)

# Finds the system it is on. for fun and debugging if needed
string(TIMESTAMP time)
message(STATUS "TIME OF BUILD: ${time}")
if(WIN32)
    message(STATUS "System: Windows")
elseif(APPLE)
    message(STATUS "System: MACOS")
    
elseif(UNIX)
    message(STATUS "System: Linux/Unix")
    
else()
    message(WARNING "OS Assessment: Unknown platform. Status: Disastrous.")
endif()

# List out found info.
message(STATUS "Architecture: ${CMAKE_SYSTEM_PROCESSOR}")

# Tries to list the compiler, otherwise throw error.
if(CMAKE_CXX_COMPILER)
    message(STATUS "Compiler Located: ${CMAKE_CXX_COMPILER}")
else()
    message(FATAL_ERROR "Compiler Reassessment: Compiler not found. Status: Calamitous.")
endif()

# These flags only work if on GCC.
set(My_Flags "-Wall -Wextra -Wold-style-cast -Wsign-promo -Woverloaded-virtual -Wctor-dtor-privacy -Wnon-virtual-dtor -Weffc++ -pedantic")
set(CMAKE_CXX_FLAGS "${My_Flags}")

### Vulkan ###
if (DEFINED VULKAN_SDK_PATH)
    set(Vulkan_INCLUDE_DIRS "${VULKAN_SDK_PATH}/Include")
    set(Vulkan_LIBRARIES "${VULKAN_SDK_PATH}/Lib")
    set(Vulkan_FOUND "True")
else()
    find_package(Vulkan REQUIRED)
    message(STATUS "Found Vulkan: $ENV{VULKAN_SDK}")
endif()

if (NOT Vulkan_FOUND)
    mesage(FATAL_ERROR "Could not find Vulkan library!")
else()
    message(STATUS "Using Vulkan library at: ${Vulkan_LIBRARIES}")
endif()

### Shaders ###
# TODO GLSL & Compile Step

### Library Type ###
option(BUILD_SHARED_LIBS "Building with shared libraries..." OFF)

### Output ###
if (BUILD_SHARED_LIBS)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/lib")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/lib")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/lib")
else()
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/stc")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/stc")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/stc")
endif()

### Directories ###
set(OCEAN_SRC_DIRECTORY    "${PROJECT_SOURCE_DIR}/Ocean/src")
set(OCEAN_VENDOR_DIRECTORY "${PROJECT_SOURCE_DIR}/Ocean/vendor")

set(GLFW_DIRECTORY         "${PROJECT_SOURCE_DIR}/Ocean/vendor/glfw/include")
set(GLM_DIRECTORY          "${PROJECT_SOURCE_DIR}/Ocean/vendor/glm")
set(TLSF_DIRECTORY         "${PROJECT_SOURCE_DIR}/Ocean/vendor/tlsf")
set(VK_MEM_ALLOC_DIRECTORY "${PROJECT_SOURCE_DIR}/Ocean/vendor/VulkanMemoryAllocator/include")

### Projects ###
# GLFW CMAKE FLAGS
set(GLFW_BUILD_WAYLAND OFF) # Wayland Support For Linux / Unix
set(GLFW_BUILD_X11 OFF)     # X11 Support For Linux / Unix
set(GLFW_BUILD_COCOA OFF)   # Cocoa Support For 
set(GLFW_BUILD_DOCS OFF)    # GLFW DoxyGen Documentation Building
add_subdirectory(Ocean/vendor/glfw)

add_subdirectory(Ocean)

add_subdirectory(Sandbox)
