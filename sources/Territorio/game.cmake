set(PROJECT_NAME Territorio)

include(sdl2.cmake)
include(assimp.cmake)

include(libs.cmake)

set(RENDER_ON ON)

include_library(eastl 3rd_party/eastl)
include_library(ecs ecs)
include_library(engine engine)
include_library(common_systems common_systems)

include_library(imgui 3rd_party/imgui)
include_library(TerritorioLib Territorio)

