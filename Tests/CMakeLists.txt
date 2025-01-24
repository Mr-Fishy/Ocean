Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.8.0 # or a later release
)

FetchContent_MakeAvailable(Catch2)

link_libraries(Ocean Catch2::Catch2)

set(CMAKE_FOLDER "Tests")

enable_testing()

set(Ocean_TESTS_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src")

add_executable(Numerics ${Ocean_TESTS_SRC}/NumericTests.cpp)

add_test(TestAll Numerics)

unset(CMAKE_FOLDER)
