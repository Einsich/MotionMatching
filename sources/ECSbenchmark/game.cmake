
set(PROJECT_NAME ECSbenchmark)

include(sdl2.cmake)
include(assimp.cmake)

include(libs.cmake)


include_library(eastl 3rd_party/eastl)
include_library(ecs ecs)
include_library(das das)
include_library(engine engine)

include_library(imgui 3rd_party/imgui)
include_library(3d_application 3rd_party/3d_application )
include_library(libDaScript 3rd_party/daScript)
include_library(ECSbenchmarkLib ECSbenchmark)

