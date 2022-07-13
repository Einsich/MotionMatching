cmake_minimum_required(VERSION 3.10)
set(MODULE_NAME ECSbenchmark)
set(MODULE_PATH ${SRC_ROOT}/${MODULE_NAME})
include(../module.cmake)
set(LibsNamesList ${LibsNamesList} ECSbenchmark PARENT_SCOPE)

include_directories(${SRC_ROOT}/das)
include_directories(${SRC_ROOT}/3rd_party/daScript/include)