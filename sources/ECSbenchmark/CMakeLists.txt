cmake_minimum_required(VERSION 3.10)
set(PROJECT_NAME ECSbenchmark)
set(PROJECT_PATH ${SRC_ROOT}/${PROJECT_NAME})


set(LibsNamesList
  ecs
  das
  libDaScript
  3d_application
  imgui
  eastl
  engine
)

include_directories(${SRC_ROOT}/das)
include_directories(${SRC_ROOT}/3rd_party/daScript/include)

include(../sdl2.cmake)
include(../assimp.cmake)


include(../exe.cmake)
