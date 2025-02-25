cmake_minimum_required(VERSION 3.11...3.22)
cmake_policy(VERSION 3.11)

find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

string(COMPARE EQUAL ${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR} Ocean_MAIN_PROJECT)

option(CMAKE_VERBOSE_MAKEFILE "Use CMake verbose make file." OFF)

option(Ocean_BUILD_DLL "Build Ocean as a dynamic library." ON)
option(Ocean_BUILD_DOCS "Generate Ocean Engine documentation target." ON)
option(Ocean_BUILD_TESTS "Build Ocean tests." Ocean_INTERNAL_BUILD_TESTS)

if (NOT DEFINED Ocean_INTERNAL_BUILD_TESTS AND Ocean_MAIN_PROJECT)
    set(Ocean_BUILD_TESTS ON)
endif ()

project("Ocean Engine")

if (Ocean_MAIN_PROJECT)

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

endif (Ocean_MAIN_PROJECT)

if (MSVC)
    message(STATUS)
    message(FATAL_ERROR "Ocean Only Supports GCC Compilers!")
endif ()

### Library Type ###

### Output ###
if (Ocean_BUILD_DLL)
    message(STATUS "Configuring Ocean as a Dynamic Library.")

    set(BUILD_SHARED_LIBS ON)

    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/lib")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/lib")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/lib")
else()
    message(STATUS "Configuring Ocean as a Static Library.")

    set(BUILD_SHARED_LIBS OFF)

    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/stc")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/stc")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin/stc")
endif()

### OpenGL ###
set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)

if (NOT OPENGL_FOUND)
    message(FATAL_ERROR "OpenGL NOT Found.")
else()
    message(STATUS "OpenGL Libraries Found.")
endif ()

### Vulkan ###
set(Vulkan_FOUND False)
find_package(Vulkan REQUIRED)

if (DEFINED ${VULKAN_SDK_PATH})
    set(Vulkan_FOUND True)
endif()

if (NOT Vulkan_FOUND)
    message(FATAL_ERROR "Vulkan descent protocol: Active. Utter failure of Building: Imminent.")
else()
    message(STATUS "Vulkan Libraries Found.")
endif()

### Shaders ###
find_program(
    GLSL_VALIDATOR glslangValidator

    HINTS
        ${Vulkan_GLSLANG_VALIDATOR_EXECUTABLE}
        /usr/bin/
        /usr/local/bin/
        ${VULKAN_SDK_PATH}/Bin/
        ${VULKAN_SDK_PATH}/Bin32/
        $ENV{VULKAN_SDK}/Bin/
        $ENV{VULKAN_SDK}/Bin32/
)

file(
    GLOB GLSL_SOURCE_FILES

    "${PROJECT_SOURCE_DIR}/assets/shaders/*.vert"
    "${PROJECT_SOURCE_DIR}/assets/shaders/*.frag"
)

foreach(GLSL ${GLSL_SOURCE_FILES})

    get_filename_component(FILE_NAME ${GLSL} NAME)
    set(SPIRV "${PROJECT_SOURCE_DIR}/assets/shaders/${FILE_NAME}.spv")

    add_custom_command(
        OUTPUT ${SPIRV}
        COMMAND ${GLSL_VALIDATOR} -V ${GLSL} -o ${SPIRV}
        DEPENDS ${GLSL}
    )

    list(APPEND SPIRV_BINARY_FILES ${SPIRV})

endforeach(GLSL)

add_custom_target(
    Shaders

    DEPENDS ${SPIRV_BINARY_FILES}
)

### Directories ###
set(OCEAN_SRC_DIRECTORY    "${PROJECT_SOURCE_DIR}/Ocean/src")

set(GLFW_DIRECTORY         "${PROJECT_SOURCE_DIR}/Ocean/vendor/glfw/include")
set(GLM_DIRECTORY          "${PROJECT_SOURCE_DIR}/Ocean/vendor/glm")
set(TLSF_DIRECTORY         "${PROJECT_SOURCE_DIR}/Ocean/vendor/tlsf")
set(STEAM_AUDIO_DIRECTORY  "${PROJECT_SOURCE_DIR}/Ocean/vendor/steamaudio/include")
set(GLAD_DIRECTORY         "${PROJECT_SOURCE_DIR}/Ocean/vendor/glad/include")
set(STB_DIRECTORY          "${PROJECT_SOURCE_DIR}/Ocean/vendor/stb")
set(CR_DIRECTORY          "${PROJECT_SOURCE_DIR}/Ocean/vendor/cr")


### Projects ###
message(STATUS)
message(STATUS "Passing off to Bob the builder")
message(STATUS)

set(CMAKE_FOLDER "Deps")

# GLFW
string(TIMESTAMP time "%M:%S")
message(STATUS "GLFW Starting (${time})...")
set(GLFW_BUILD_WAYLAND OFF) # NOTE: This may be required for MacOS
add_subdirectory(Ocean/vendor/glfw EXCLUDE_FROM_ALL)
message(STATUS)

# GLM
string(TIMESTAMP time "%M:%S")
message(STATUS "GLM Starting (${time})...")
set(CMAKE_POLICY_DEFAULT_CMP0077 "NEW")
if (BUILD_SHARED_LIBS)
    set(GLM_BUILD_LIBRARY ON)
else()
    set(GLM_BUILD_LIBRARY OFF)
endif()
set(GLM_ENABLE_CXX_17 ON)
set(GLM_BUILD_TESTS OFF)
add_subdirectory(Ocean/vendor/glm EXCLUDE_FROM_ALL)
message(STATUS)

add_subdirectory(Ocean/vendor/glad EXCLUDE_FROM_ALL)

# Steam Audio
string(TIMESTAMP time "%M:%S")
message(STATUS "Steam Audio Starting (${time})...")
add_subdirectory(Ocean/vendor/steamaudio EXCLUDE_FROM_ALL)
message(STATUS)


#Flecs ECS
string(TIMESTAMP time "%M:%S")
message(STATUS "Flecs Starting (${time})...")
add_subdirectory(Ocean/vendor/flecs EXCLUDE_FROM_ALL)
message(STATUS)

# GLSLang
string(TIMESTAMP time "%M:%S")
message(STATUS "glslang Starting (${time})...")
option(ENABLE_SPVREMAPPER OFF)
option(ENABLE_GLSLANG_BINARIES OFF)
set(ENABLE_HLSL OFF)
option(ENABLE_OPT OFF)
option(BUILD_TESTING OFF)
option(BUILD_EXTERNAL OFF)
if(IOS)
    option(ENABLE_PCH OFF)
endif ()
add_subdirectory(Ocean/vendor/glslang EXCLUDE_FROM_ALL)
message(STATUS)
unset(CMAKE_FOLDER)

set(
    OCEAN_COMPILE_FLAGS

    -DCMAKE_EXPORT_COMPILE_COMMANDS=1
    -Wall
    -Wextra
    -Wold-style-cast
    -Wsign-promo
    -Woverloaded-virtual
    -Wctor-dtor-privacy
    -Wnon-virtual-dtor
    -Weffc++
    -pedantic
)

string(TIMESTAMP time "%M:%S")
message(STATUS "Ocean Starting (${time})...")
message(STATUS)
add_subdirectory(Ocean)

if (Ocean_BUILD_DOCS)
    
    string(TIMESTAMP time "%M:%S")
    add_subdirectory(Docs)
    message(STATUS "Doxygen Setup Started (${time})...")
    message(STATUS)

endif (Ocean_BUILD_DOCS)

if (Ocean_BUILD_TESTS)

    enable_testing()

    add_subdirectory(Ocean/tests)

endif (Ocean_BUILD_TESTS)

string(TIMESTAMP time "%M:%S")
message(STATUS "Sandbox Starting (${time})...")
message(STATUS)
add_subdirectory(Sandbox)

string(TIMESTAMP time "%H:%M:%S")
message(STATUS "All required steps Completed, EOF for ${PROJECT_NAME} at time: ${time}\n")
message(STATUS "Clever little rats. I will not forget what you have done here today.")
