project(Ocean VERSION 0.1 DESCRIPTION "The Core Engine")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Collect the library files
file(GLOB_RECURSE Ocean_Glob RELATIVE_PATH src/ *.cpp)

add_library(
    ${CMAKE_PROJECT_NAME}

    ${Ocean_Glob}
)

include_directories(
    src
    vendor

    
)

target_link_libraries(
    ${CMAKE_PROJECT_NAME}

    GLFW
)
