cmake_minimum_required(VERSION 3.14)
project(Assessment)

set(CMAKE_CXX_STANDARD 17)

enable_testing()

find_package(GTest CONFIG REQUIRED)


add_executable(AssessmentTests
    mainTest.cpp
    graph.cpp
    boruvka.cpp
    union_find.cpp
    lca.cpp
    kkt.cpp
)

target_include_directories(AssessmentTests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(AssessmentTests PRIVATE GTest::gtest GTest::gtest_main)

# ---------------------------------------------------
# Enable automatic test discovery
# ---------------------------------------------------
include(GoogleTest)
gtest_discover_tests(AssessmentTests)
