cmake_minimum_required(VERSION 3.19)

# set the project name
project(adventofcode20)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include(${CMAKE_BINARY_DIR}/conan_paths.cmake)
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()
enable_testing()
find_package(Threads REQUIRED) 
find_package(folly REQUIRED) 
# add the executable
add_executable(day15 day15_bench.cc)
target_link_libraries(day15 PRIVATE ${CONAN_LIBS}  folly double-conversion glog)
