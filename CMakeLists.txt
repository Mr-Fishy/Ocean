cmake_minimum_required(VERSION 3.28)

project(OceanEngine)

#Finds the system it is on. for fun and debugging if needed
string(TIMESTAMP time)
message(STATUS "TIME OF BUILD: ${time}")
if(WIN32)
    message(STATUS "System: Windows")
elseif(APPLE)
    message(STATUS "System: MACOS")
    
elseif(UNIX)
    message(STATUS "System: Linux/Unix")
    
else()
    message(WARNING "OS Assessment: Unknown platform. Status: Disastrous.")
endif()

#List out found info.
message(STATUS "Architecture: ${CMAKE_SYSTEM_PROCESSOR}")

#Tries to list the compiler, otherwise throw error.
if(CMAKE_CXX_COMPILER)
    message(STATUS "Compiler Located: ${CMAKE_CXX_COMPILER}")
else()
    message(FATAL_ERROR "Compiler Reassessment: Compiler not found. Status: Calamitous.")
endif()

# Directories


# Sub Projects
add_subdirectory(Ocean/vendor/glfw)

add_subdirectory(Ocean)
add_subdirectory(Sandbox)
