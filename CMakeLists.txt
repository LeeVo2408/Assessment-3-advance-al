


cmake_minimum_required(VERSION 3.14)
project(Assessment LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

# --- GoogleTest setup ---
# If using vcpkg (which your folder suggests)
find_package(GTest CONFIG REQUIRED)

# --- Core source files (your algorithms) ---
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

# --- Main executable (your main program) ---
add_executable(AssessmentMain
    main.cpp
    ${SRC_FILES}
    ${HEADER_FILES}
)
target_include_directories(AssessmentMain PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# --- Test executables ---
add_executable(AssessmentTests
    boruvkaTest.cpp
    randomisedTest.cpp
    ${SRC_FILES}
    ${HEADER_FILES}
)

target_include_directories(AssessmentTests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(AssessmentTests PRIVATE GTest::gtest GTest::gtest_main)

# --- Automatically discover and register Google Tests ---
include(GoogleTest)
gtest_discover_tests(AssessmentTests)
