cmake_minimum_required(VERSION 3.15)
project(CacheSimulator)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

file(GLOB SOURCES "src/*.cpp")

add_executable(CacheSimulator ${SOURCES})