
if(WIN32)
  set(ADDITIONAL_LIBS ${ADDITIONAL_LIBS} freetype)

  add_library(freetype STATIC IMPORTED)
  set_target_properties(freetype PROPERTIES
          IMPORTED_LOCATION_DEBUG "${CMAKE_BINARY_DIR}/../freetype-d.lib"
          IMPORTED_LOCATION_RELWITHDEBINFO "${CMAKE_BINARY_DIR}/../freetype-r.lib"
          IMPORTED_LOCATION_RELEASE "${CMAKE_BINARY_DIR}/../freetype-r.lib"
          LINKER_LANGUAGE CXX)
else()
  find_package(freetype REQUIRED)
endif()