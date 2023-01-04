

include(sdl2.cmake)

include(libs.cmake)


include_library(eastl 3rd_party/eastl FALSE)
include_library(ecsLib 3rd_party/ecs/sources/ecs FALSE)
include_library(optick 3rd_party/optick/src TRUE)
include_library(FileWatcher 3rd_party/FileWatcher/source TRUE)
include_library(das das FALSE)
set(RENDER_ON OFF)
include_library(engine engine TRUE)

include_library(imgui 3rd_party/imgui TRUE)
include_library(libDaScript 3rd_party/daScript FALSE)
include_library(ECSbenchmarkLib ECSbenchmark FALSE)

