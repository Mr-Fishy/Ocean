link_libraries(Ocean)

set(CMAKE_FOLDER "Tests")

enable_testing()

add_executable(Memory ${CMAKE_CURRENT_SOURCE_DIR}/Memory.cpp)

add_test(TestAll Memory)

unset(CMAKE_FOLDER)
