set(DOXYGEN_SKIP_DOT TRUE)
find_package(Doxygen)

if (DOXYGEN_FOUND)

    file(GLOB_RECURSE Ocean_DOXYGEN_INPUT_FILES ${CMAKE_SOURCE_DIR}/Ocean/src/*.hpp LIST_DIRECTORIES TRUE)

    foreach(path IN LISTS Ocean_DOXYGEN_INPUT_FILES)
        string(APPEND Ocean_DOXYGEN_INPUT " \\\n\"${path}\"")
    endforeach()
    string(APPEND Ocean_DOXYGEN_INPUT " \\\n\"${CMAKE_CURRENT_SOURCE_DIR}/INSTALL.md\"")
    string(APPEND Ocean_DOXYGEN_INPUT " \\\n\"${CMAKE_SOURCE_DIR}/README.md\"")

    set(Ocean_DOXYGEN_OUTPUT "${PROJECT_SOURCE_DIR}/bin/docs")

    cmake_policy(SET CMP0135 NEW)
    include(FetchContent)
    FetchContent_Declare(
        doxygen-awesome-css
        URL https://github.com/jothepro/doxygen-awesome-css/archive/refs/heads/main.zip
    )
    FetchContent_MakeAvailable(doxygen-awesome-css)
    FetchContent_GetProperties(doxygen-awesome-css SOURCE_DIR AWESOME_CSS_DIR)

    configure_file(Doxyfile.in Doxyfile @ONLY)

    add_custom_target(
        Docs ALL

        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating HTML Ocean documentation"
        VERBATIM
    )

else (DOXYGEN_FOUND)

    message(WARNING "Doxygen needs to be installed to generate Ocean's documentation.")

endif (DOXYGEN_FOUND)
