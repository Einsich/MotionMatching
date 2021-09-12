cmake_minimum_required(VERSION 3.10)
#need
#LIBRARY
#LIB_FOLDER
project(Engine)

message("path-${CMAKE_SOURCE_DIR}")
set(LIB_FOLDER "${CMAKE_SOURCE_DIR}/Common/Sources")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-m64 -Wall -Wextra -Wno-pragma-pack -Wno-deprecated-declarations")
if(BUILD_TYPE STREQUAL "dbg")
    set(CMAKE_BUILD_TYPE  "Debug")
elseif(BUILD_TYPE STREQUAL "rel")
    set(CMAKE_BUILD_TYPE  "Release")
endif()
message("path-${LIB_FOLDER}")

file(GLOB_RECURSE ENGINE_SOURCES ${LIB_FOLDER}/*.cpp)
file(GLOB_RECURSE ENGINE_C_SOURCES ${LIB_FOLDER}/*.c)


#include(GNUInstallDirs)
include_directories(${LIB_FOLDER})
add_library(Engine OBJECT ${ENGINE_SOURCES} ${ENGINE_C_SOURCES})


