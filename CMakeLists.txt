cmake_minimum_required(VERSION 3.10)

# Set Project Name
project(Janus VERSION 1.0)

file(GLOB SOURCES "src/*.cpp")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE Debug)

add_executable(Janus ${SOURCES})


target_include_directories(Janus PUBLIC "${PROJECT_SOURCE_DIR}/include")

