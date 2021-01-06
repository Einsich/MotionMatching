cmake_minimum_required(VERSION 3.7)

set(PROJECT_NAME MainApplication.exe)
project(${PROJECT_NAME})

find_package(glfw3 3.3 REQUIRED)
find_package(SDL2 REQUIRED)
find_library(SDL2_IMAGE_LIBRARY SDL2_image)

find_package(assimp REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
message("SDLInclude: ${SDL2_INCLUDE_DIRS}")
message("Assimp libraries: ${ASSIMP_LIBRARIES}")
set(ROOT /home/yan/Documents/MotionMatching/Sources)
include_directories(${ROOT})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -g")
set(TINYLINK "-lX11 -lpthread -lSDL2  -lSDL2_image ")
set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${TINYLINK}")

file(GLOB IMGUI_SRC "${ROOT}/imgui/*.cpp")

file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "${ROOT}/CommonCode/*.cpp")
file(GLOB_RECURSE C_SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "${ROOT}/glad/*.c")

add_executable(${PROJECT_NAME} ${SOURCES} ${C_SOURCES} ${IMGUI_SRC} ${ROOT}/MainApplication/main.cpp)

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARY} assimp glfw ${CMAKE_DL_LIBS})



