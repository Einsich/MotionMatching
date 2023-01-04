

include(sdl2.cmake)
include(assimp.cmake)

include(libs.cmake)

set(RENDER_ON ON)
include_library(eastl 3rd_party/eastl FALSE)
include_library(ecsLib 3rd_party/ecs/sources/ecs FALSE)
include_library(optick 3rd_party/optick/src TRUE)
include_library(FileWatcher 3rd_party/FileWatcher/source TRUE)
include_library(engine engine TRUE)
include_library(common_systems common_systems FALSE)

include_library(imgui 3rd_party/imgui TRUE)
include_library(TerritorioLib Territorio FALSE)

