cmake_minimum_required(VERSION 3.10)
project(opengl LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# SDL3 via find_package (ou via vendored, se você quiser depois)
find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared)

find_package(OpenGL REQUIRED)

add_subdirectory(src)