cmake_minimum_required(VERSION 3.20)
#need
#LIBRARY
#LIB_FOLDER
project(Engine-${BUILD_TYPE})

message("path-${CMAKE_SOURCE_DIR}")
set(LIB_FOLDER "${CMAKE_SOURCE_DIR}/../Common/Sources")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-pragma-pack -Wno-deprecated-declarations")
if(BUILD_TYPE STREQUAL "dbg")
    set(CMAKE_BUILD_TYPE  "Debug")
elseif(BUILD_TYPE STREQUAL "dev")
    set(CMAKE_BUILD_TYPE  "Debug")
elseif(BUILD_TYPE STREQUAL "rel")
    set(CMAKE_BUILD_TYPE  "Release")
endif()
message("path-${LIB_FOLDER}")

file(GLOB_RECURSE ENGINE_SOURCES ${LIB_FOLDER}/*.cpp)
file(GLOB_RECURSE ENGINE_C_SOURCES ${LIB_FOLDER}/*.c)


#include(GNUInstallDirs)
include_directories(${LIB_FOLDER})
add_library(${PROJECT_NAME} OBJECT ${ENGINE_SOURCES} ${ENGINE_C_SOURCES})


