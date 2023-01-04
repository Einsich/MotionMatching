set(MODULE_NAME das)
set(MODULE_PATH ${SRC_ROOT}/${MODULE_NAME})


if (IS_AOT_COMPILER)
  set(SOURCES_DIRS aot das_modules)
else()
  set(SOURCES_DIRS . das_modules)
endif()

include(../module.cmake)

include_directories(${SRC_ROOT}/3rd_party/daScript/include)
include_directories(${SRC_ROOT}/3rd_party/ecs/include)
include_directories(${SRC_ROOT}/3rd_party/eastl/include)
include_directories(${SRC_ROOT}/engine)