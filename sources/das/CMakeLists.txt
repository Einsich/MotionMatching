cmake_minimum_required(VERSION 3.10)
set(MODULE_NAME das)
set(MODULE_PATH ${SRC_ROOT}/${MODULE_NAME})

include(../module.cmake)
set(LibsNamesList ${LibsNamesList} das PARENT_SCOPE)

set(DAS_BUILD_TEST NO PARENT_SCOPE)
set(DAS_BUILD_PROFILE NO PARENT_SCOPE)
set(DAS_BUILD_TUTORIAL NO PARENT_SCOPE)
set(DAS_BUILD_TOOLS NO PARENT_SCOPE)

include_directories(${SRC_ROOT}/3rd_party/daScript/include)