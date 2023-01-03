

macro(ecs_pull_module)
  
  set(ECS_PULLS )
  file(GLOB_RECURSE MODULE_ECS_SOURCES ${MODULE_PATH}/*.inl)
 
  foreach(PATH ${MODULE_ECS_SOURCES})
    
    set(FILE_NAME )
    get_filename_component(FILE_NAME ${PATH} NAME_WE)
    #message(${FILE_NAME})
    set(ECS_PULLS "${ECS_PULLS} PULL(variable_pull_${FILE_NAME})")
  endforeach()

  string(COMPARE EQUAL "${ECS_PULLS}" "" result)
  if (NOT result)
    file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/ecs_pulls.cpp
      "#include <ecs/ecs.h>\n"
      "#define PULL_VARS ${ECS_PULLS}\n"
      "#define PULL ECS_PULL_DECLARATION\n"
      "PULL_VARS\n"
      "#undef PULL\n"
      "#define PULL(PULL_VAR) + PULL_VAR\n"
      "size_t variable_pull_${MODULE_NAME} = 0 + PULL_VARS ;\n"
      "#undef PULL\n"
    )
    set (MODULE_ECS_SOURCES ${CMAKE_CURRENT_BINARY_DIR}/ecs_pulls.cpp)

  endif()
endmacro()


macro(ecs_gather_modules OUT_VAR)

  set(ECS_PULLS )
  foreach(PULL ${ARGN})
    set(ECS_PULLS "${ECS_PULLS} PULL(variable_pull_${PULL})")
  endforeach()
  
  file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/ecs_gather_modules.cpp
    "#include <ecs/ecs.h>\n"
    "#define PULL_VARS ${ECS_PULLS}\n"
    "#define PULL ECS_PULL_DECLARATION\n"
    "PULL_VARS\n"
    "#undef PULL\n"
    "#define PULL(PULL_VAR) + PULL_VAR\n"
    "size_t ${OUT_VAR} = 0 + PULL_VARS ;\n"
    "#undef PULL\n"
  )
  set (MODULE_ECS_SOURCES ${MODULE_ECS_SOURCES} ${CMAKE_CURRENT_BINARY_DIR}/ecs_gather_modules.cpp)

endmacro()