cmake_minimum_required(VERSION 4.2.1) # or whatever version you want to use
# it is also possible to use a range of allowable versions
# e.g.
# cmake_minimum_required(VERSION 3.15...3.28)

set(WORKING_NAME Demo03)

project(${WORKING_NAME} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCE "src/*.cpp")
include_directories(src/include)

add_executable(${WORKING_NAME} ${SOURCE})
target_include_directories(${WORKING_NAME} PRIVATE src/include)