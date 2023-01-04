cmake_minimum_required(VERSION 3.10)

set(GAME_NAME ${GAME})
set(IS_AOT_COMPILER false)
if(AOT_COMPILER STREQUAL "yes")
    message("AOT COMPILER!")
    set(IS_AOT_COMPILER true)
    set(GAME_NAME ${GAME}-aot)
endif()
message(PROJECT = ${GAME_NAME})
project(${GAME_NAME})
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

set(DAS_FLEX_BISON_DISABLED ON)
set(DAS_CLANG_BIND_DISABLED ON)
set(DAS_LLVM_DISABLED ON)
set(DAS_QUIRREL_DISABLED ON)
set(DAS_HV_DISABLED ON)
set(DAS_GLFW_DISABLED ON)
set(DAS_IMGUI_DISABLED ON)
set(DAS_BGFX_DISABLED ON)
set(DAS_XBYAK_DISABLED ON)
set(DAS_MINFFT_DISABLED ON)
set(DAS_SOUND_DISABLED ON)
set(DAS_STDDLG_DISABLED ON)
set(DAS_STBIMAGE_DISABLED ON)
set(DAS_STBTRUETYPE_DISABLED ON)
set(DAS_SFML_DISABLED ON)


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
set(EXE_SOURCES )

if(IS_AOT_COMPILER)
    set(EXE_SOURCES ${SRC_ROOT}/3rd_party/daScript/utils/daScript/main.cpp)

    set(ADDITIONAL_LIBS ${ADDITIONAL_LIBS} libDaScript eastl)
    add_subdirectory(3rd_party/daScript)
    add_subdirectory(3rd_party/eastl)
    
    include_directories(${SRC_ROOT}/3rd_party/daScript/include)
    include_directories(${SRC_ROOT}/3rd_party/ecs/include) #only for ska map
    include_directories(${SRC_ROOT}/3rd_party/eastl/include)
endif()

set(GAME_CMAKE ${SRC_ROOT}/${GAME}/game.cmake)

add_executable(${GAME_NAME} ${EXE_SOURCES})
include(${GAME_CMAKE})
target_link_libraries(${GAME_NAME} ${ADDITIONAL_LIBS})

