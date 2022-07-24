set(MODULE_NAME ArcadeLib)
set(MODULE_PATH ${SRC_ROOT}/Arcade)

include_directories(${SRC_ROOT}/engine)
include_directories(${SRC_ROOT}/ecs)
include_directories(${SRC_ROOT}/3rd_party/eastl/include)
include_directories(${SRC_ROOT}/3rd_party/include)
include_directories(${SRC_ROOT}/3rd_party/imgui)
include(../module.cmake)
