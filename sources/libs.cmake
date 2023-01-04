

macro(include_library lib_name lib_path skip_in_aot)

  message("lib_name=${lib_name} skip_in_aot = ${skip_in_aot}\n")
  set(SKIP_AOT_COMPILATION ${skip_in_aot})

  if ((NOT IS_AOT_COMPILER) OR (NOT SKIP_AOT_COMPILATION))

    message("lib_name=${lib_name} included \n")
    add_subdirectory(${lib_path})
    target_link_libraries(${GAME_NAME} ${lib_name})
  endif()
endmacro()

