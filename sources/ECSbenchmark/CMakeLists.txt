set(MODULE_NAME ECSbenchmarkLib)
set(MODULE_PATH ${SRC_ROOT}/ECSbenchmark)

include_directories(${SRC_ROOT}/engine)
include_directories(${SRC_ROOT}/3rd_party/eastl/include)
include_directories(${SRC_ROOT}/3rd_party/ecs/include)
include_directories(${SRC_ROOT}/3rd_party/include)
include_directories(${SRC_ROOT}/3rd_party/imgui)
include_directories(${SRC_ROOT}/3rd_party/daScript/include)
include_directories(${SRC_ROOT}/das)

include(../ecs_pull.cmake)
set(SOURCES )

if (IS_AOT_COMPILER)
  set(SOURCES_DIRS aot das_modules)
else()
  ecs_pull_module()
  ecs_gather_modules(ecs_pull; ECSbenchmarkLib)
  set(SOURCES_DIRS . das_modules)
endif()

if(NOT IS_AOT_COMPILER)
  execute_process(COMMAND ${AOT_COMPILER_PATH} -aot ${MODULE_PATH}/init.das ${MODULE_PATH}/init.das.cpp -project ${DAS_PROJECT_PATH} -dasroot ${DAS_ROOT})

  endif()

message(${SOURCES_DIRS})
set(OBJECT_LIB true)
include(../module.cmake)
