project(Sandbox)

file(GLOB_RECURSE sandbox_HEADER ./src/*.hpp)
file(GLOB_RECURSE sandbox_SOURCE ./src/*.cpp)

source_group("App Files" FILES ${sandbox_HEADER})
source_group("App Files" FILES ${sandbox_SOURCE})

add_executable(
    ${PROJECT_NAME} 

    ${sandbox_HEADER} ${sandbox_SOURCE}
)

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE

    src/

    ${OCEAN_SRC_DIRECTORY}
    ${Vulkan_INCLUDE_DIRS}
)

target_link_libraries(
    ${PROJECT_NAME}
    
    PRIVATE Ocean
)

if (GNU)

    set(SANDBOX_DEBUG_OPTIONS "OC_DEBUG=1 -Wall -Wextra -Wold-style-cast -Wsign-promo -Woverloaded-virtual -Wctor-dtor-privacy -Wnon-virtual-dtor -Weffc++ -pedantic")

    target_compile_options(
        ${PROJECT_NAME}
        PRIVATE "$<$<CONFIG:DEBUG>:${SANDBOX_DEBUG_OPTIONS}>"
    )

endif()