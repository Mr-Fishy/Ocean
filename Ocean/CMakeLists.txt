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
    PRIVATE

    src
    vendor

    ${TLSF_DIRECTORY}

    ${Vulkan_INCLUDE_DIRS}
)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE

    GLFW
    glm::glm
    ${Vulkan_LIBRARIES}
)
message(STATUS "All required steps Completed, EOF for ${PROJECT_NAME} at time: ${time}\n")
