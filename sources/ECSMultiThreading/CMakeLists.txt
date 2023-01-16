set(MODULE_NAME ECSMultiThreadingLib)
set(MODULE_PATH ${SRC_ROOT}/ECSMultiThreading)

include_directories(${SRC_ROOT}/engine)
include_directories(${SRC_ROOT}/3rd_party/eastl/include)
include_directories(${SRC_ROOT}/3rd_party/ecs/include)
include_directories(${SRC_ROOT}/3rd_party/include)
include_directories(${SRC_ROOT}/3rd_party/imgui)
include_directories(${SRC_ROOT}/3rd_party/optick/include)

include(../ecs_pull.cmake)
set(SOURCES )

if (IS_AOT_COMPILER)
  set(SOURCES_DIRS )
else()
  ecs_pull_module()
  ecs_gather_modules(ecs_pull; ECSMultiThreadingLib)
  set(SOURCES_DIRS . )
endif()



set(OBJECT_LIB true)
include(../module.cmake)
