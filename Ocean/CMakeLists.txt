project(Ocean)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED)

# Collect the library files
file(GLOB_RECURSE Ocean_Glob RELATIVE src *.cpp)

add_library(
    ${PROJECT_NAME}

    ${Ocean_Glob}
)

include_directories(src)

target_link_libraries(
    ${PROJECT_NAME}

    glfw
)
