cmake_minimum_required(VERSION 3.7)

set(PROJECT_NAME MainApplication.exe)
project(${PROJECT_NAME})

find_package(SDL2 REQUIRED)
find_package(assimp REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

get_filename_component(PARENT_DIR ${CMAKE_SOURCE_DIR} DIRECTORY)
set(ROOT ${PARENT_DIR}/Sources)

include_directories(${ROOT})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(TINYLINK "-lX11 -lpthread -lSDL2 ")
set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${TINYLINK}")
if(BUILD_TYPE STREQUAL "dbg")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -g -O0 -DDEBUG -fsanitize=address -fsanitize-address-use-after-scope -fno-omit-frame-pointer -fsanitize=leak -fsanitize=undefined -fsanitize=bounds-strict")
    message("DebugBuild")
elseif(BUILD_TYPE STREQUAL "dev")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -DDEBUG")
    message("DevelopBuild")
elseif(BUILD_TYPE STREQUAL "rel")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx2 -O3")
    message("ReleaseBuild")
endif()

file(GLOB IMGUI_SRC "${ROOT}/imgui/*.cpp")
file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "${ROOT}/CommonCode/*.cpp")
file(GLOB_RECURSE C_SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "${ROOT}/glad/*.c")
file(GLOB_RECURSE PROJ_SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "${ROOT}/MainApplication/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES} ${C_SOURCES} ${IMGUI_SRC} ${PROJ_SOURCES})

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} assimp ${CMAKE_DL_LIBS})



