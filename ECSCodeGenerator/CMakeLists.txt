cmake_minimum_required(VERSION 3.7)

set(PROJECT_NAME ECSCodeGenerator.exe)
project(${PROJECT_NAME})

set(ROOT /home/yan/Documents/MotionMatching/Sources)
include_directories(${ROOT})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -g")


file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "${ROOT}/ECSCodeGenerator/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})
#target_link_libraries(${PROJECT_NAME} ${CMAKE_DL_LIBS})

