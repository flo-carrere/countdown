cmake_minimum_required(VERSION 3.24)

project(countdown)

add_executable(app)
target_sources(app PUBLIC main.cpp)
target_compile_options(app PUBLIC -fdiagnostics-color=always)
