Include(FetchContent)
FetchContent_Declare(
Catch2
GIT_REPOSITORY https://github.com/catchorg/Catch2.git
GIT_TAG        v3.8.0 # or a later release
)
FetchContent_MakeAvailable(Catch2)
list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)

include(CTest)

include(Catch)

set_target_properties(
    Catch2 Catch2WithMain PROPERTIES

    FOLDER "Tests"
)

set(CMAKE_FOLDER "Tests")

file(GLOB Ocean_TEST_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")

add_executable(
    OceanTests
    
    ${Ocean_TEST_HEADERS}
    ${CMAKE_CURRENT_SOURCE_DIR}/OceanTests.cpp
)

target_include_directories(
    OceanTests

    PRIVATE ${Ocean_TESTS_SRC}
)

target_link_libraries(
    OceanTests

    PRIVATE Ocean
    PRIVATE Catch2::Catch2WithMain
)

add_test(NAME TestAll COMMAND OceanTests)

catch_discover_tests(OceanTests)

unset(CMAKE_FOLDER)
