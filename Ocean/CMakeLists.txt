project(Ocean VERSION 0.1 DESCRIPTION "The Core Engine")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# GLob all of the following files in the directories.

# Root Files
file(GLOB Ocean_ROOT_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/src/Ocean/*.hpp)

foreach(FILE_PATH ${Ocean_ROOT_HEADER})
    message(STATUS "File: ${FILE_PATH}")
endforeach()

# Core Files
file(GLOB Ocean_CORE_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/src/Ocean/Core/*.hpp)
file(GLOB Ocean_CORE_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/src/Ocean/Core/*.cpp)

message(STATUS)
foreach(FILE_PATH ${Ocean_CORE_HEADER})
    message(STATUS "File: ${FILE_PATH}")
endforeach()
foreach(FILE_PATH ${Ocean_CORE_SOURCE})
    message(STATUS "File: ${FILE_PATH}")
endforeach()

file(GLOB Ocean_INPUT_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/src/Ocean/Core/Input/*.hpp)
file(GLOB Ocean_INPUT_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/src/Ocean/Core/Input/*.cpp)
file(GLOB Ocean_PRIMI_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/src/Ocean/Core/Primitives/*.hpp)
file(GLOB Ocean_PRIMI_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/src/Ocean/Core/Primitives/*.cpp)
file(GLOB Ocean_TYPES_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/src/Ocean/Core/Types/*.hpp)
file(GLOB Ocean_TYPES_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/src/Ocean/Core/Types/*.cpp)

message(STATUS)
foreach(FILE_PATH ${Ocean_INPUT_HEADER})
    message(STATUS "File: ${FILE_PATH}")
endforeach()
foreach(FILE_PATH ${Ocean_INPUT_SOURCE})
    message(STATUS "File: ${FILE_PATH}")
endforeach()
foreach(FILE_PATH ${Ocean_PRIMI_HEADER})
    message(STATUS "File: ${FILE_PATH}")
endforeach()
foreach(FILE_PATH ${Ocean_PRIMI_SOURCE})
    message(STATUS "File: ${FILE_PATH}")
endforeach()
foreach(FILE_PATH ${Ocean_TYPES_HEADER})
    message(STATUS "File: ${FILE_PATH}")
endforeach()
foreach(FILE_PATH ${Ocean_TYPES_SOURCE})
    message(STATUS "File: ${FILE_PATH}")
endforeach()

# Platform Files
file(GLOB Ocean_PLAT_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/src/Platform/*.hpp)
file(GLOB Ocean_PLAT_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/src/Platform/*.cpp)

message(STATUS)
foreach(FILE_PATH ${Ocean_PLAT_HEADER})
    message(STATUS "File: ${FILE_PATH}")
endforeach()
foreach(FILE_PATH ${Ocean_PLAT_SOURCE})
    message(STATUS "File: ${FILE_PATH}")
endforeach()

# Renderer Files
file(GLOB Ocean_REND_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/src/Renderer/*.hpp)
file(GLOB Ocean_REND_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/src/Renderer/*.cpp)

message(STATUS)
foreach(FILE_PATH ${Ocean_REND_HEADER})
    message(STATUS "File: ${FILE_PATH}")
endforeach()
foreach(FILE_PATH ${Ocean_REND_SOURCE})
    message(STATUS "File: ${FILE_PATH}")
endforeach()

# Vendor Files (Non-Libraries)
file(GLOB tlsf_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/vendor/tlsf/*.h)
file(GLOB tlsf_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/vendor/tlsf/*.c)

# Group all of the files per their source for IDE / Editor

source_group("Root File" FILES ${Ocean_ROOT_HEADER})

source_group("Core Files" FILES ${Ocean_CORE_HEADER})
source_group("Core Files" FILES ${Ocean_CORE_SOURCE})

source_group("Input Files" FILES ${Ocean_INPUT_HEADER})
source_group("Input Files" FILES ${Ocean_INPUT_SOURCE})
source_group("Primitive Files" FILES ${Ocean_PRIMI_HEADER})
source_group("Primitive Files" FILES ${Ocean_PRIMI_SOURCE})
source_group("Type Files" FILES ${Ocean_TYPES_HEADER})
source_group("Type Files" FILES ${Ocean_TYPES_SOURCE})

source_group("Platform Files" FILES ${Ocean_PLAT_HEADER})
source_group("Platform Files" FILES ${Ocean_PLAT_SOURCE})

source_group("Renderer Files" FILES ${Ocean_REND_HEADER})
source_group("Renderer Files" FILES ${Ocean_REND_SOURCE})

source_group("tlsf Files" FILES ${tlsf_FILES})

add_library(
    ${PROJECT_NAME}

    ${Ocean_ROOT_HEADER}

    ${Ocean_CORE_HEADER} ${Ocean_CORE_SOURCE}
    ${Ocean_INPUT_HEADER} ${Ocean_INPUT_SOURCE}
    ${Ocean_PRIMI_HEADER} ${Ocean_PRIMI_SOURCE}
    ${Ocean_TYPES_HEADER} ${Ocean_TYPES_SOURCE}

    ${Ocean_PLAT_HEADER}  ${Ocean_PLAT_SOURCE}

    ${Ocean_REND_HEADER} ${Ocean_REND_SOURCE}

    ${tlsf_HEADER} ${tlsf_SOURCE}
)

target_include_directories(
    ${PROJECT_NAME}
    
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/

    PRIVATE ${TLSF_DIRECTORY}
    PRIVATE ${Vulkan_INCLUDE_DIRS}
)

target_link_libraries(
    ${PROJECT_NAME}

    PRIVATE glfw

    PUBLIC glm::glm
    PUBLIC ${Vulkan_LIBRARIES}
)

# target_compile_options(
#     ${PROJECT_NAME}
#     PRIVATE "$<$<CONFIG:DEBUG>:${OCEAN_COMPILE_FLAGS}>"
# )
