project(Ocean VERSION 0.1 DESCRIPTION "The Core Engine")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Collect the library files
file(GLOB_RECURSE Ocean_Glob RELATIVE_PATH src/ *.cpp)

add_library(
    ${PROJECT_NAME}

    ${Ocean_Glob}
)

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC

    src
    vendor

    ${GLFW_DIRECTORY}
    ${GLM_DIRECTORY}
    ${TLSF_DIRECTORY}
    ${VK_MEM_ALLOC_DIRECTORY}

    ${Vulkan_INCLUDE_DIRS}
)

target_link_libraries(
    ${PROJECT_NAME}

    GLFW

    ${Vulkan_LIBRARIES}
)
