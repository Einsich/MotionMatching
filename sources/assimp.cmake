
if(WIN32)
  set(ADDITIONAL_LIBS ${ADDITIONAL_LIBS} assimp)

  add_library(assimp STATIC IMPORTED)
  set_target_properties(assimp PROPERTIES
          IMPORTED_LOCATION_DEBUG "${CMAKE_BINARY_DIR}/../assimp.lib"
          IMPORTED_LOCATION_RELWITHDEBINFO "${CMAKE_BINARY_DIR}/../assimp.lib"
          IMPORTED_LOCATION_RELEASE "${CMAKE_BINARY_DIR}/../assimp.lib"
          LINKER_LANGUAGE CXX)
else()
  find_package(assimp REQUIRED)
endif()