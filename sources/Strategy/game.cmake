
set(PROJECT_NAME Strategy)

include(sdl2.cmake)
include(assimp.cmake)

include(libs.cmake)


include_library(eastl 3rd_party/eastl)
include_library(ecs ecs)
include_library(engine engine)
include_library(common_systems common_systems)

include_library(imgui 3rd_party/imgui)
include_library(3d_application 3rd_party/3d_application)
include_library(StrategyLib Strategy)

