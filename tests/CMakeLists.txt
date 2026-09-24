find_package(GTest QUIET)

if (NOT GTest_FOUND)
    include(FetchContent)

    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
    set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.tar.gz
    )
    FetchContent_MakeAvailable(googletest)
endif()

add_executable(countdown_tests solver_test.cpp)
target_link_libraries(countdown_tests PRIVATE countdown_core GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(countdown_tests)
