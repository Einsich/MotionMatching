set(MODULE_NAME ECSbenchmarkLib)
set(MODULE_PATH ${SRC_ROOT}/ECSbenchmark)

include_directories(${SRC_ROOT}/engine)
include_directories(${SRC_ROOT}/ecs)
include_directories(${SRC_ROOT}/3rd_party/eastl/include)
include_directories(${SRC_ROOT}/3rd_party/3d_application)
include_directories(${SRC_ROOT}/3rd_party/imgui)
include_directories(${SRC_ROOT}/3rd_party/daScript/include)
include_directories(${SRC_ROOT}/das)

include(../module.cmake)
