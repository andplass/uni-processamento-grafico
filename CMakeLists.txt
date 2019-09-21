cmake_minimum_required(VERSION 3.14)
project(processamento_grafico)

set(CMAKE_CXX_STANDARD 14)

include_directories(libs)
include_directories(libs/GLEW)
include_directories(libs/GLEW/include)
include_directories(libs/GLEW/include/GL)
include_directories(libs/GLFW)
include_directories(libs/GLFW/include)
include_directories(libs/GLFW/include/GLFW)

add_executable(processamento_grafico
        libs/GLEW/include/GL/eglew.h
        libs/GLEW/include/GL/glew.h
        libs/GLEW/include/GL/glxew.h
        libs/GLEW/include/GL/wglew.h
        libs/GLFW/include/GLFW/glfw3.h
        libs/GLFW/include/GLFW/glfw3native.h
        main.cpp)
