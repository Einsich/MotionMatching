cmake_minimum_required(VERSION 3.10)
set(MODULE_NAME Test)
set(MODULE_PATH ${SRC_ROOT}/${MODULE_NAME})
include(../module.cmake)

set(LibsNamesList ${LibsNamesList} Test PARENT_SCOPE)