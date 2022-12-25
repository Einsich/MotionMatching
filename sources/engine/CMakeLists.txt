set(MODULE_NAME engine)
set(MODULE_PATH ${SRC_ROOT}/${MODULE_NAME})

option(RENDER_ON "" ON)

include_directories(${SRC_ROOT}/3rd_party/include)
include_directories(${SRC_ROOT}/3rd_party/eastl/include)
include_directories(${SRC_ROOT}/3rd_party/imgui)
include_directories(${SRC_ROOT}/3rd_party/ecs/include)

macro(add_folder folder)
  file(GLOB_RECURSE TMP_SOURCES RELATIVE ${MODULE_PATH} ${folder}/*.cpp)
  set(MODULE_SOURCES ${MODULE_SOURCES} ${TMP_SOURCES})
endmacro()

add_folder(application)
add_folder(data_block)
add_folder(ecs)
add_folder(memory)
add_folder(parallel)
if (RENDER_ON)
  add_folder(render)
  add_folder(ui)
  add_folder(resources)
else()
  add_folder(render_stub)
endif()
set(MODULE_SOURCES ${MODULE_SOURCES} driver/glad.c)

add_folder(profiler)
add_folder(serialization)

set(MODULE_SOURCES ${MODULE_SOURCES} camera.cpp common.cpp input.cpp main.cpp transform.cpp transform2d.cpp)

include_directories(${MODULE_PATH})

add_library(${MODULE_NAME} OBJECT ${MODULE_SOURCES})