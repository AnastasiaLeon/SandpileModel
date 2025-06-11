cmake_minimum_required(VERSION 3.10)
project(SandpileModel)

set(CMAKE_CXX_STANDARD 17) 

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

include_directories(sandpile_model)

add_executable(SandpileModel 
    src/main.cpp
    src/sandpile.cpp
    src/bmp.cpp
)

set(OUTPUT_DIR "${CMAKE_SOURCE_DIR}/outputDir")
file(MAKE_DIRECTORY ${OUTPUT_DIR})