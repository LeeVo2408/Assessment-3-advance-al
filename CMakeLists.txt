cmake_minimum_required(VERSION 3.14)
project(Assessment LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

# --- Automatically fetch GoogleTest ---
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# --- Source files ---
set(SRC_FILES
    graph.cpp
    union_find.cpp
    kkt.cpp
    lca.cpp
    boruvka.cpp
)

set(HEADER_FILES
    graph.hpp
    union_find.hpp
    kkt.hpp
    lca.hpp
    boruvka.hpp
)

# --- Main executable ---
add_executable(AssessmentMain
    main.cpp
    ${SRC_FILES}
    ${HEADER_FILES}
)
target_include_directories(AssessmentMain PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# --- Test executables ---
add_executable(AssessmentTests
    ${SRC_FILES}
    ${HEADER_FILES}
)
target_include_directories(AssessmentTests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(AssessmentTests PRIVATE GTest::gtest GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(AssessmentTests)
