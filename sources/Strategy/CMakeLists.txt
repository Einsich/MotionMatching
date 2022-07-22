cmake_minimum_required(VERSION 3.10)
set(PROJECT_NAME Strategy)
set(PROJECT_PATH ${SRC_ROOT}/${PROJECT_NAME})

set(LibsNamesList
  ecs
  3d_application
  imgui
  eastl
  engine
  common_systems
)

include(../sdl2.cmake)
include(../assimp.cmake)

include(../exe.cmake)