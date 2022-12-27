
set(MODULE_NAME optick)
set(MODULE_PATH ${SRC_ROOT}/3rd_party/optick/src)
include_directories(${SRC_ROOT}/3rd_party/optick/include)
include(../../../module.cmake)
target_compile_definitions(optick PUBLIC OPTICK_ENABLE_GPU_VULKAN=0)
target_compile_definitions(optick PUBLIC OPTICK_ENABLE_GPU_D3D12=0)
