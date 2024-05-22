cmake_minimum_required(VERSION 3.5)
project(MyProject)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

file(GLOB SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_executable(main ${SRC_FILES} main.cpp)

target_link_libraries(main sfml-graphics sfml-window sfml-system)
