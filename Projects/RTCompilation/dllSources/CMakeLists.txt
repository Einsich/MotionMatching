if (WIN32)
    set(CMAKE_VER 3.20)
else()
    set(CMAKE_VER 3.10)
endif()

cmake_minimum_required(VERSION ${CMAKE_VER})

set(ADDITIONAL_LIBS "")
if(WIN32)

    set(PARENT_DIR ${CMAKE_SOURCE_DIR}/../../..)
    add_subdirectory(../../../Common ../../../Builds/win/TestDll/dev/Engine)
    set(CMAKE_CXX_COMPILER "C:/Program Files/LLVM/bin/clang++.exe")
    set(CMAKE_C_COMPILER "C:/Program Files/LLVM/bin/clang.exe")
    set(PROJECT_NAME ${PROJECT}-${BUILD_TYPE})
    set(ADDITIONAL_LIBS winmm.lib Version.lib Setupapi.lib)
    set (BuildDir "${PARENT_DIR}/Builds")
    add_library(SDL2 STATIC IMPORTED)
    set_target_properties(SDL2 PROPERTIES
        IMPORTED_LOCATION_RELEASE "${BuildDir}/win/libSDL2.a"
        IMPORTED_LOCATION_DEBUG "${BuildDir}/win/libSDL2.a"
        LINKER_LANGUAGE C)
    add_library(assimp STATIC IMPORTED)
    set_target_properties(assimp PROPERTIES
            IMPORTED_LOCATION_DEBUG "${BuildDir}/win/assimp-vc142-mt.lib"
            IMPORTED_LOCATION_RELEASE "${BuildDir}/win/assimp-vc142-mt.lib"
            LINKER_LANGUAGE C)
else()
    set(PROJECT_NAME ${PROJECT}-${BUILD_TYPE}.exe)
    set(ADDITIONAL_LIBS -ldl)
endif()

project(${PROJECT_NAME})

if(BUILD_TYPE STREQUAL "dev")
    set(CMAKE_BUILD_TYPE  "Debug")
elseif(BUILD_TYPE STREQUAL "rel")
    set(CMAKE_BUILD_TYPE  "Release")
endif()


set(PROJECT_ROOT ${CMAKE_SOURCE_DIR})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-pragma-pack -Wno-deprecated-declarations")

include_directories(${PROJECT_ROOT}/../Sources)
include_directories(${PROJECT_ROOT}/../../../Common/Sources)
message("${PROJECT_ROOT}")
#file(GLOB_RECURSE PROJECT_SOURCES  ${PROJECT_ROOT}/*.cpp)
#file(GLOB_RECURSE PROJECT_C_SOURCES ${PROJECT_ROOT}/*.c)
add_library(${PROJECT_NAME} SHARED f.cpp ../Sources/listener.cpp)
target_link_libraries(${PROJECT_NAME} SDL2 assimp Engine-${BUILD_TYPE} ${ADDITIONAL_LIBS})




