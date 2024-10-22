project(Sandbox)

add_executable(${PROJECT_NAME} src/Sandbox.cpp)

target_link_libraries(
    ${PROJECT_NAME}

    Ocean
)
