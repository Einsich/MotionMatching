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

include_directories(${MODULE_PATH})

add_library(${MODULE_NAME} STATIC ${SOURCES})

set(SOURCES )
set(SOURCES_DIRS )
set(SOURCES_REC_DIRS )