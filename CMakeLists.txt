cmake_minimum_required(VERSION 3.16)
project(PI_project LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SFML 2 REQUIRED COMPONENTS graphics window system)

add_executable(sfml2_test
    main.cpp
)

target_link_libraries(sfml2_test PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
)
