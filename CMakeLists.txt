cmake_minimum_required(VERSION 3.19)

project(sa-asi-loader VERSION 1.1 LANGUAGES CXX)

set(CMAKE_SHARED_LIBRARY_PREFIX "")
set(CMAKE_CXX_STANDARD 17)

add_library(vorbisFile SHARED dllmain.cpp)