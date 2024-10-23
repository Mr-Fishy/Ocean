project(Sandbox)

add_executable(${PROJECT_NAME} src/Sandbox.cpp)

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC

    ${OCEAN_SRC_DIRECTORY}
    ${OCEAN_VENDOR_DIRECTORY}

    ${GLM_DIRECTORY}

    ${Vulkan_INCLUDE_DIRS}
)

target_link_libraries(
    ${PROJECT_NAME}

    Ocean
)
