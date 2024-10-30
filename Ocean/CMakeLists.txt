project(Ocean VERSION 0.1 DESCRIPTION "The Core Engine")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Root Files
file(GLOB Ocean_ROOT_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/src/Ocean/*.hpp)

# Core Files
file(GLOB Ocean_CORE_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/src/Ocean/Core/*.hpp)
file(GLOB Ocean_CORE_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/src/Ocean/Core/*.cpp)

file(GLOB_RECURSE Ocean_INPUT_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/src/Ocean/Core/Input/*.hpp)
file(GLOB_RECURSE Ocean_INPUT_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/src/Ocean/Core/Input/*.cpp)
file(GLOB_RECURSE Ocean_PRIMI_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/src/Ocean/Primitives/*.hpp)
file(GLOB_RECURSE Ocean_PRIMI_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/src/Ocean/Primitives/*.cpp)
file(GLOB_RECURSE Ocean_TYPES_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/src/Ocean/Types/*.hpp)
file(GLOB_RECURSE Ocean_TYPES_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/src/Ocean/Types/*.cpp)

# Platform Files
file(GLOB Ocean_PLAT_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/src/Platform/*.hpp)
file(GLOB Ocean_PLAT_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/src/Platform/*.cpp)

# Renderer Files
file(GLOB Ocean_REND_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/src/Renderer/*.hpp)
file(GLOB Ocean_REND_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/src/Renderer/*.cpp)

source_group("Root File" FILES ${Ocean_ROOT_HEADER})

source_group("Core Files" FILES ${Ocean_CORE_HEADER})
source_group("Core Files" FILES ${Ocean_CORE_SOURCE})

source_group("Input Files" FILES ${Ocean_INPUT_HEADER})
source_group("Input Files" FILES ${Ocean_INPUT_SOURCE})
source_group("Primitive Files" FILES ${Ocean_PRIMI_HEADER})
source_group("Primitive Files" FILES ${Ocean_PRIMI_SOURCE})
source_group("Type Files" FILES ${Ocean_TYPES_HEADER})
source_group("Type Files" FILES ${Ocean_TYPES_SOURCE})

source_group("Platform Files" FILES ${Ocean_PLAT_HEADER})
source_group("Platform Files" FILES ${Ocean_PLAT_SOURCE})

source_group("Renderer Files" FILES ${Ocean_REND_HEADER})
source_group("Renderer Files" FILES ${Ocean_REND_SOURCE})

add_library(
    ${PROJECT_NAME}

    ${Ocean_ROOT_HEADER}

    ${Ocean_CORE_HEADER} ${Ocean_CORE_SOURCE}
    ${Ocean_INPUT_HEADER} ${Ocean_INPUT_SOURCE}
    ${Ocean_PRIMI_HEADER} ${Ocean_PRIMI_SOURCE}
    ${Ocean_TYPES_HEADER} ${Ocean_TYPES_SOURCE}

    ${Ocean_PLAT_HEADER}  ${Ocean_PLAT_SOURCE}

    ${Ocean_REND_HEADER} ${Ocean_REND_SOURCE}
)

target_include_directories(
    ${PROJECT_NAME}
    
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/

    PRIVATE ${TLSF_DIRECTORY}
    PRIVATE ${Vulkan_INCLUDE_DIRS}
)

target_link_libraries(
    ${PROJECT_NAME}

    PUBLIC glm::glm
    PRIVATE glfw
    PRIVATE ${Vulkan_LIBRARIES}
)

set(OCEAN_DEBUG_OPTIONS "OC_DEBUG=1")

target_compile_options(
    ${PROJECT_NAME}
    PRIVATE "$<$<CONFIG:DEBUG>:${OCEAN_DEBUG_OPTIONS}>"
)
