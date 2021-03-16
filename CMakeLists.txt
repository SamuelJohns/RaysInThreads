cmake_minimum_required(VERSION 2.8)

project(RayTracerConsole)

SET(CMAKE_CXX_FLAGS -pthread)

add_executable(main main_console.cpp raytracerconsole.cpp)

