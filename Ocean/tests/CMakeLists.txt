include(CTest)

file(GLOB TestsToRun ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

set(CMAKE_FOLDER "Tests")

function(auto_add_test name)

    add_executable(${name} "${CMAKE_CURRENT_SOURCE_DIR}/${name}.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/base/Tests.cpp")

    set(CMAKE_BUILD_TYPE "Debug")

    target_link_libraries(
        ${name}

        PRIVATE Ocean
    )

    target_compile_options(
        ${name}

        PRIVATE -coverage
    )

    target_link_options(
        ${name}

        PRIVATE -coverage
    )

    add_test(NAME ${name} COMMAND ${name} --verbose)

endfunction()

foreach (source ${TestsToRun})

    get_filename_component(name ${source} NAME_WE)

    message(STATUS "Adding Ocean Test: ${name}")
    auto_add_test(${name})
    
endforeach()

add_custom_target(TestAll COMMAND ${CMAKE_CTEST_COMMAND} --verbose)

unset(CMAKE_FOLDER)