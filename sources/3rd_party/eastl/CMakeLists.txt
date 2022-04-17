#-------------------------------------------------------------------------------------------
# Copyright (C) Electronic Arts Inc.  All rights reserved.
#-------------------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.10)
project(${MODULE_NAME})

file(GLOB_RECURSE MODULE_SOURCES ${MODULE_PATH}/*.cpp)
#-------------------------------------------------------------------------------------------
# Defines
#-------------------------------------------------------------------------------------------
add_definitions(-D_CHAR16T)
add_definitions(-D_CRT_SECURE_NO_WARNINGS)
add_definitions(-D_SCL_SECURE_NO_WARNINGS)
add_definitions(-DEASTL_OPENSOURCE=1)

include_directories(${MODULE_PATH}/include)

add_library(${MODULE_NAME} OBJECT ${MODULE_SOURCES})




