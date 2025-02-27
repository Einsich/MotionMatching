foreach(DIR ${SOURCES_REC_DIRS})
  file(GLOB_RECURSE TMP_SOURCES RELATIVE ${MODULE_PATH} ${DIR}/*.cpp)
  #message(${TMP_SOURCES})
  set(SOURCES ${SOURCES} ${TMP_SOURCES})
endforeach()

foreach(DIR ${SOURCES_DIRS})
  file(GLOB TMP_SOURCES RELATIVE ${MODULE_PATH} ${DIR}/*.cpp)
  #message(${TMP_SOURCES})
  set(SOURCES ${SOURCES} ${TMP_SOURCES})
endforeach()

if (NEED_AOT_GENERATION)
  foreach(DIR ${SOURCES_DAS_DIRS})
    file(GLOB TMP_SOURCES RELATIVE ${MODULE_PATH} ${DIR}/*.das)
    #message(${TMP_SOURCES})
    foreach(DAS_FILE ${TMP_SOURCES})
      set(DAS_CPP_FILE ${CMAKE_CURRENT_BINARY_DIR}/${DAS_FILE}.cpp)
      execute_process(COMMAND ${AOT_COMPILER_PATH} -aot ${MODULE_PATH}/${DAS_FILE} ${DAS_CPP_FILE} -project ${DAS_PROJECT_PATH} -dasroot ${DAS_ROOT})
      set(SOURCES ${SOURCES} ${DAS_CPP_FILE})
    endforeach()
  endforeach()
endif()

include_directories(${MODULE_PATH})
if (OBJECT_LIB)
add_library(${MODULE_NAME} OBJECT ${SOURCES})
else()
add_library(${MODULE_NAME} STATIC ${SOURCES})
endif()
set(SOURCES )
set(SOURCES_DIRS )
set(SOURCES_REC_DIRS )