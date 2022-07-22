
if(WIN32)
  set(ADDITIONAL_LIBS ${ADDITIONAL_LIBS} SDL2 imm32)
  set(BuildDir "${ROOT}/Builds")
  add_library(SDL2 STATIC IMPORTED)
  set_target_properties(SDL2 PROPERTIES
      IMPORTED_LOCATION_RELEASE "${BuildDir}/libSDL2.a"
      IMPORTED_LOCATION_DEBUG "${BuildDir}/libSDL2.a"
      LINKER_LANGUAGE CXX)
else()
  find_package(SDL2 REQUIRED)
  include_directories(${SDL2_INCLUDE_DIRS})
endif()