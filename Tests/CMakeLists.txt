link_libraries(Ocean)

add_executable(Memory ${CMAKE_CURRENT_SOURCE_DIR}/Memory.cpp)


set_target_properties(
    Memory PROPERTIES

    FOLDER "Tests"
)
