
set(PROJECT_NAME Arcade)

include(sdl2.cmake)
include(assimp.cmake)

include(libs.cmake)


set(RENDER_ON ON)
include_library(eastl 3rd_party/eastl)
include_library(ecsLib 3rd_party/ecs/sources/ecs)
include_library(optick 3rd_party/optick/src)
include_library(FileWatcher 3rd_party/FileWatcher/source)
include_library(engine engine)
include_library(common_systems common_systems)

include_library(imgui 3rd_party/imgui)
include_library(ArcadeLib Arcade)

