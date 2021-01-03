cmake_minimum_required(VERSION 3.7)

set(PROJECT_NAME ShaderPrecompiler.exe)
project(${PROJECT_NAME})

find_package(glfw3 3.3 REQUIRED)

find_package(SDL2 REQUIRED)
find_package(assimp REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
message("SDLInclude: ${SDL2_INCLUDE_DIRS}")
message("Assimp libraries: ${ASSIMP_LIBRARIES}")
set(ROOT /home/yan/Documents/MotionMatching/Sources)
include_directories(${ROOT})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -g")

#file(GLOB IMGUI_SRC src/imgui/*.cpp)

set(SOURCES ${ROOT}/ShaderPrecompiler/main.cpp ${ROOT}/CommonCode/shader_factory.cpp ${ROOT}/CommonCode/shader.cpp
${ROOT}/Glad/src/glad.c)

add_executable(${PROJECT_NAME} ${SOURCES} ${IMGUI_SRC})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} assimp glfw ${CMAKE_DL_LIBS})

