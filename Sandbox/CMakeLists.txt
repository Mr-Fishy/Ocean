project(Sandbox)

add_executable(${PROJECT_NAME} src/Sandbox.cpp)

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE

    ${OCEAN_SRC_DIRECTORY}
    ${OCEAN_VENDOR_DIRECTORY}

    ${Vulkan_INCLUDE_DIRS}
)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE

    Ocean
)
message(STATUS "All required steps Completed, EOF for ${PROJECT_NAME} at time: ${time}\n")