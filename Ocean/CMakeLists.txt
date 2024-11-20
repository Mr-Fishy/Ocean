project(Ocean VERSION 0.1 DESCRIPTION "The Core Engine")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# GLob all of the following files in the directories.

# Root Files
file(GLOB Ocean_ROOT_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/src/Ocean/*.hpp)

# Core Files
file(GLOB Ocean_CORE_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/src/Ocean/Core/*.hpp)
file(GLOB Ocean_CORE_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/src/Ocean/Core/*.cpp)

file(GLOB Ocean_INPUT_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/src/Ocean/Core/Input/*.hpp)
file(GLOB Ocean_INPUT_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/src/Ocean/Core/Input/*.cpp)
file(GLOB Ocean_PRIMI_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/src/Ocean/Core/Primitives/*.hpp)
file(GLOB Ocean_PRIMI_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/src/Ocean/Core/Primitives/*.cpp)
file(GLOB Ocean_TYPES_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/src/Ocean/Core/Types/*.hpp)
file(GLOB Ocean_TYPES_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/src/Ocean/Core/Types/*.cpp)

# Platform Files
file(GLOB Ocean_PLAT_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/src/Platform/*.hpp)
file(GLOB Ocean_PLAT_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/src/Platform/*.cpp)

# Renderer Files
file(GLOB Ocean_REND_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/src/Renderer/*.hpp)
file(GLOB Ocean_REND_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/src/Renderer/*.cpp)

file(GLOB Ocean_RENCOMP_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/src/Renderer/Components/*.hpp)
file(GLOB Ocean_RENCOMP_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/src/Renderer/Components/*.cpp)

# Vendor Files (Non-Libraries)
## TLSF
file(GLOB tlsf_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/vendor/tlsf/*.h)
file(GLOB tlsf_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/vendor/tlsf/*.c)

## Steam Audio
file (GLOB steamaudio_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/vendor/steamaudio/include/*.h)

# Group all of the files per their source for IDE / Editor

source_group("Root Header" FILES ${Ocean_ROOT_HEADER})

source_group("Core Headers" FILES ${Ocean_CORE_HEADER})
source_group("Core Sources" FILES ${Ocean_CORE_SOURCE})

source_group("Input Headers" FILES ${Ocean_INPUT_HEADER})
source_group("Input Sources" FILES ${Ocean_INPUT_SOURCE})
source_group("Primitive Headers" FILES ${Ocean_PRIMI_HEADER})
source_group("Primitive Sources" FILES ${Ocean_PRIMI_SOURCE})
source_group("Type Headers" FILES ${Ocean_TYPES_HEADER})
source_group("Type Sources" FILES ${Ocean_TYPES_SOURCE})

source_group("Platform Headers" FILES ${Ocean_PLAT_HEADER})
source_group("Platform Sources" FILES ${Ocean_PLAT_SOURCE})

source_group("Renderer Headers" FILES ${Ocean_REND_HEADER})
source_group("Renderer Sources" FILES ${Ocean_REND_SOURCE})

source_group("Renderer Component Headers" FILES ${Ocean_RENCOMP_HEADER})
source_group("Renderer Component Sources" FILES ${Ocean_RENCOMP_SOURCE})

source_group("tlsf Headers" FILES ${tlsf_HEADER})
source_group("tlsf Sources" FILES ${tlsf_SOURCE})

source_group("Steam Audio Headers" FILES ${steamaudio_HEADER})

add_library(
    ${PROJECT_NAME}

    ${Ocean_ROOT_HEADER}

    ${Ocean_CORE_HEADER} ${Ocean_CORE_SOURCE}
    ${Ocean_INPUT_HEADER} ${Ocean_INPUT_SOURCE}
    ${Ocean_PRIMI_HEADER} ${Ocean_PRIMI_SOURCE}
    ${Ocean_TYPES_HEADER} ${Ocean_TYPES_SOURCE}

    ${Ocean_PLAT_HEADER}  ${Ocean_PLAT_SOURCE}

    ${Ocean_REND_HEADER} ${Ocean_REND_SOURCE}
    ${Ocean_RENCOMP_HEADER} ${Ocean_RENCOMP_SOURCE}

    ${tlsf_HEADER} ${tlsf_SOURCE}
    ${steamaudio_HEADER}
)

target_include_directories(
    ${PROJECT_NAME}
    
    PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/src/
)

target_include_directories(
    ${PROJECT_NAME} SYSTEM

    PRIVATE ${TLSF_DIRECTORY}
    PUBLIC  ${GLM_DIRECTORY}

    PRIVATE ${Vulkan_INCLUDE_DIRS}
)

target_link_libraries(
    ${PROJECT_NAME}

    PRIVATE glfw
    PUBLIC SteamAudio

    PUBLIC glm::glm-header-only
    PUBLIC ${Vulkan_LIBRARIES}
)

target_compile_options(
    ${PROJECT_NAME} PRIVATE

    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>: ${OCEAN_COMPILE_FLAGS}>

    $<$<CXX_COMPILER_ID:MSVC>: /W4>
)

target_compile_definitions(
    ${PROJECT_NAME}

    PRIVATE $<$<PLATFORM_ID:Windows>: WIN32_LEAN_AND_MEAN>
    PUBLIC $<$<CONFIG:Debug>: OC_DEBUG>
    PUBLIC $<$<CONFIG:Release>: OC_RELEASE>
)
