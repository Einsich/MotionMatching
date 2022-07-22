cmake_minimum_required(VERSION 3.10)

project(dummy)

if(WIN32)
    set(CMAKE_CXX_COMPILER "C:/Program Files/LLVM/bin/clang++.exe")
    set(CMAKE_C_COMPILER "C:/Program Files/LLVM/bin/clang.exe")
    set(ADDITIONAL_LIBS winmm.lib Version.lib Setupapi.lib)
else()
    set(ADDITIONAL_LIBS -ldl)
endif()


set(ROOT ${CMAKE_SOURCE_DIR}/..)
set(SRC_ROOT ${CMAKE_SOURCE_DIR})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-m64 -Wall -Wextra -Wno-pragma-pack -Wno-deprecated-declarations -g")


if(BUILD_TYPE STREQUAL "dbg")
    set(CMAKE_BUILD_TYPE "Debug")
else()
    #if (WIN32)
    #    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
    #endif()
    set(CMAKE_BUILD_TYPE  "Release")
endif()

if(BUILD_TYPE STREQUAL "rel")
    add_definitions(-DRELEASE)
endif()



set(INCLUDES 3rd_party/3d_application;3rd_party/imgui;3rd_party/eastl/include;ecs;engine;common_systems;)
foreach(DEPENDENCY ${INCLUDES})
  include_directories(${CMAKE_SOURCE_DIR}/${DEPENDENCY})
endforeach()


set(ALL_LIBS
  ecs
  das
  3rd_party/3d_application
  3rd_party/imgui
  3rd_party/eastl
  3rd_party/daScript
  engine
  common_systems
)


foreach(LIB ${ALL_LIBS})
  add_subdirectory(${SRC_ROOT}/${LIB})
endforeach()


add_subdirectory(${SRC_ROOT}/${GAME})