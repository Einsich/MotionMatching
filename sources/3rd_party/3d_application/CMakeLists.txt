cmake_minimum_required(VERSION 3.10)
set(MODULE_NAME 3d_application)
set(MODULE_PATH ${SRC_ROOT}/3rd_party/${MODULE_NAME})

include(../../module.cmake)
set(LibsNamesList ${LibsNamesList} 3d_application PARENT_SCOPE)
