
if(WIN32)
  set(ADDITIONAL_LIBS ${ADDITIONAL_LIBS} assimp)
  set (BuildDir "${ROOT}/Builds")
  add_library(assimp STATIC IMPORTED)
  set_target_properties(assimp PROPERTIES
          IMPORTED_LOCATION_DEBUG "${BuildDir}/assimp.lib"
          IMPORTED_LOCATION_RELEASE "${BuildDir}/assimp.lib"
          LINKER_LANGUAGE CXX)
else()
  find_package(assimp REQUIRED)
endif()