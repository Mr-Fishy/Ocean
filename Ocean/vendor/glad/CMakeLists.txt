project(glad VERSION 2.0.8 LANGUAGES C)

# GLAD Source Files
file(GLOB_RECURSE glad_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")
file(GLOB glad_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")

add_library(
    ${PROJECT_NAME}

    ${glad_HEADER} ${glad_SOURCE}
)

target_include_directories(
    ${PROJECT_NAME}

    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
