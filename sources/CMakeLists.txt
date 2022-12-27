cmake_minimum_required(VERSION 3.10)

project(${GAME})
if(WIN32)
    set(ADDITIONAL_LIBS winmm.lib Version.lib Setupapi.lib)
else()
    set(ADDITIONAL_LIBS -ldl)
endif()


set(SRC_ROOT ${CMAKE_SOURCE_DIR})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-m64 -Wall -Wextra -Wno-pragma-pack -Wno-deprecated-declarations -g")


set(DAS_BUILD_TEST NO)
set(DAS_BUILD_PROFILE NO)
set(DAS_BUILD_TUTORIAL NO)
set(DAS_BUILD_TOOLS NO)

if(BUILD_TYPE STREQUAL "dbg")
    set(CMAKE_BUILD_TYPE "Debug")
else()
    if(BUILD_TYPE STREQUAL "dev")
        set(CMAKE_BUILD_TYPE "RelWithDebInfo")
    else()
    #if (WIN32)
    #    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
    #endif()
        set(CMAKE_BUILD_TYPE "Release")
    endif()
endif()

if(BUILD_TYPE STREQUAL "rel")
    add_definitions(-DRELEASE)
endif()

include(3rd_party/daScript/CMakeCommon.txt)
SETUP_COMPILER()

set(GAME_CMAKE ${SRC_ROOT}/${GAME}/game.cmake)

if(EXISTS ${GAME_CMAKE})
    add_executable(${GAME})
    include(${GAME_CMAKE})
    target_link_libraries(${GAME} ${ADDITIONAL_LIBS})
else()
    add_subdirectory(${SRC_ROOT}/${GAME})
endif()
