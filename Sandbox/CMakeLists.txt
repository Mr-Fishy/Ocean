project(Sandbox)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE sandbox_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp)
file(GLOB_RECURSE sandbox_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

source_group("App Files" FILES ${sandbox_HEADER})
source_group("App Files" FILES ${sandbox_SOURCE})

add_executable(
    ${PROJECT_NAME} 

    ${sandbox_HEADER} ${sandbox_SOURCE}
)

target_include_directories(
    ${PROJECT_NAME}
    
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/
)

target_include_directories(
    ${PROJECT_NAME} SYSTEM

    PRIVATE ${Vulkan_INCLUDE_DIRS}
)

target_link_libraries(
    ${PROJECT_NAME}

    PRIVATE Ocean
)

target_compile_options(
    ${PROJECT_NAME} PRIVATE

    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>: ${OCEAN_COMPILE_FLAGS}>

    $<$<CXX_COMPILER_ID:MSVC>: /W4>
)
