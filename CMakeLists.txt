cmake_minimum_required(VERSION 3.16)
project(PI_project LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Place executable in project root
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

# Gather all source files
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)
add_executable(PI_project ${SOURCES})

# Platform-specific SFML setup
if(UNIX)
    # Linux: automatically find SFML in standard locations
    find_path(SFML_INCLUDE_DIR NAMES SFML/System.hpp PATHS /usr/include /usr/local/include /opt/sfml-2.6/include)
    find_library(SFML_SYSTEM_LIB NAMES sfml-system PATHS /usr/lib /usr/local/lib /opt/sfml-2.6/lib)
    find_library(SFML_WINDOW_LIB NAMES sfml-window PATHS /usr/lib /usr/local/lib /opt/sfml-2.6/lib)
    find_library(SFML_GRAPHICS_LIB NAMES sfml-graphics PATHS /usr/lib /usr/local/lib /opt/sfml-2.6/lib)

    if(NOT SFML_INCLUDE_DIR OR NOT SFML_SYSTEM_LIB OR NOT SFML_WINDOW_LIB OR NOT SFML_GRAPHICS_LIB)
        message(FATAL_ERROR "Could not find SFML 2.6 on Linux. Make sure SFML headers and libraries exist.")
    endif()

    target_include_directories(PI_project PRIVATE ${SFML_INCLUDE_DIR})
    target_link_libraries(PI_project PRIVATE ${SFML_SYSTEM_LIB} ${SFML_WINDOW_LIB} ${SFML_GRAPHICS_LIB})

elseif(WIN32)
    # Windows: use SFML CMake targets
    set(SFML_DIR "C:/SFML/lib/cmake/SFML")  # Adjust to your SFML install path
    set(CMAKE_PREFIX_PATH ${SFML_DIR})
    find_package(SFML 2.6 REQUIRED COMPONENTS system window graphics)
    target_link_libraries(PI_project PRIVATE SFML::System SFML::Window SFML::Graphics)
endif()
