# cmake_minimum_required(VERSION 3.14)
# project(Assessment)

# set(CMAKE_CXX_STANDARD 20)

# enable_testing()

# find_package(GTest CONFIG REQUIRED)


# add_executable(AssessmentTests
#     mainTest.cpp
#     graph.cpp
#     boruvka.cpp
#     union_find.cpp
#     lca.cpp
#     kkt.cpp
# )

# target_include_directories(AssessmentTests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
# target_link_libraries(AssessmentTests PRIVATE GTest::gtest GTest::gtest_main)

# # ---------------------------------------------------
# # Enable automatic test discovery
# # ---------------------------------------------------
# include(GoogleTest)
# gtest_discover_tests(AssessmentTests)

# ///

cmake_minimum_required(VERSION 3.14)
project(mst CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CTEST_OUTPUT_ON_FAILURE ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
)

set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
set(BUILD_GTEST ON  CACHE BOOL "" FORCE)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

configure_file(${CMAKE_SOURCE_DIR}/mediumEWG.txt ${CMAKE_BINARY_DIR}/mediumEWG.txt COPYONLY)

add_library(mst STATIC
  boruvka.cpp
  graph.cpp
  kkt.cpp
  lca.cpp
  union_find.cpp
)
target_include_directories(mst PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(mst_tests main.cpp)
target_link_libraries(mst_tests PRIVATE mst GTest::gtest_main)

include(CTest)
enable_testing()
include(GoogleTest)
gtest_discover_tests(mst_tests)

