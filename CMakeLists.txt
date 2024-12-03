cmake_minimum_required(VERSION 3.0.0)
project(simple_openGL_game VERSION 0.1.0)
cmake_policy(SET CMP0072 NEW) 

find_package(OpenGL REQUIRED)

add_executable(simple_openGL_game 
    src/config.h
    src/config.cpp
    src/main.cpp 
    src/glad.c 
    src/triangle_mesh_1.h
    src/triangle_mesh_1.cpp
    src/triangle_mesh_2.h
    src/triangle_mesh_2.cpp
    src/material.cpp
    src/linear_algebra.cpp
)

target_include_directories(simple_openGL_game PRIVATE dependencies)

target_link_libraries(simple_openGL_game
    glfw
    OpenGL::GL
)