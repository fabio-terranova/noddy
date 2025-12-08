cmake_minimum_required(VERSION 3.16...3.28)

project(noddy)

find_package(OpenGL REQUIRED)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(external/glfw)

include_directories(external/glad/include)

add_executable(noddy
    src/main.cpp
    external/glad/src/glad.c
)

target_link_libraries(noddy glfw OpenGL::GL)

