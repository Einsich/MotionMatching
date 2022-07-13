#-------------------------------------------------------------------------------------------
# Copyright (C) Electronic Arts Inc.  All rights reserved.
#-------------------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.10)
set(MODULE_NAME eastl)
set(MODULE_PATH ${SRC_ROOT}/3rd_party/${MODULE_NAME})

#-------------------------------------------------------------------------------------------
# Defines
#-------------------------------------------------------------------------------------------
add_definitions(-D_CHAR16T)
add_definitions(-D_CRT_SECURE_NO_WARNINGS)
add_definitions(-D_SCL_SECURE_NO_WARNINGS)
add_definitions(-DEASTL_OPENSOURCE=1)

include(../../module.cmake)
set(LibsNamesList ${LibsNamesList} eastl PARENT_SCOPE)