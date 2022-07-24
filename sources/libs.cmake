

macro(include_library lib_name lib_path)
  add_subdirectory(${lib_path})
  target_link_libraries(${PROJECT_NAME} ${lib_name})
endmacro()

