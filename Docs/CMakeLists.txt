find_package(Doxygen OPTIONAL_COMPONENTS dot)

if (DOXYGEN_FOUND)

    file(GLOB_RECURSE Ocean_DOXYGEN_INPUT_HEADER_FILES ${CMAKE_SOURCE_DIR}/Ocean/src/*.hpp LIST_DIRECTORIES TRUE)
    file(GLOB_RECURSE Ocean_DOXYGEN_INPUT_SOURCE_FILES ${CMAKE_SOURCE_DIR}/Ocean/src/*.cpp LIST_DIRECTORIES TRUE)

    foreach(path IN LISTS Ocean_DOXYGEN_INPUT_HEADER_FILES)
        string(APPEND Ocean_DOXYGEN_INPUT " \\\n\"${path}\"")
    endforeach()

    foreach(path IN LISTS Ocean_DOXYGEN_INPUT_SOURCE_FILES)
        string(APPEND Ocean_DOXYGEN_INPUT " \\\n\"${path}\"")
    endforeach()

    file(GLOB Ocean_DOXYGEN_MARKDOWN_FILES ${CMAKE_SOURCE_DIR}/Docs/*.md LIST_DIRECTORIES TRUE)

    foreach(path IN LISTS Ocean_DOXYGEN_MARKDOWN_FILES)
        string(APPEND Ocean_DOXYGEN_INPUT " \\\n\"${path}\"")
    endforeach()

    set(Ocean_DOXYGEN_MAIN_PAGE "${CMAKE_SOURCE_DIR}/README.md")

    string(APPEND Ocean_DOXYGEN_INPUT " \\\n\"${Ocean_DOXYGEN_MAIN_PAGE}\"")

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
        DEPENDS ${Ocean_DOXYGEN_INPUT_HEADER_FILES} ${Ocean_DOXYGEN_INPUT_SOURCE_FILES}
        COMMENT "Generating HTML Ocean documentation"
        VERBATIM
    )

else (DOXYGEN_FOUND)

    message(WARNING "Doxygen needs to be installed to generate Ocean's documentation.")

endif (DOXYGEN_FOUND)
