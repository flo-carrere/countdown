cmake_minimum_required(VERSION 3.24)

project(countdown LANGUAGES CXX)

option(BUILD_TESTS "Build the unit tests" OFF)

add_library(countdown_core STATIC solver.cpp)
target_include_directories(countdown_core PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_compile_features(countdown_core PUBLIC cxx_std_17)
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(countdown_core PRIVATE -fdiagnostics-color=always)
endif()

add_executable(app main.cpp)
target_link_libraries(app PRIVATE countdown_core)

if (BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
