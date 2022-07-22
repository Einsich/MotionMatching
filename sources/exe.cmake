
project(${PROJECT_NAME})

set(SOURCES )
set(C_SOURCES )

file(GLOB_RECURSE SOURCES ${PROJECT_PATH}/*.cpp)
file(GLOB_RECURSE C_SOURCES ${PROJECT_PATH}/*.c)

include_directories(${PROJECT_PATH})

add_executable(${PROJECT_NAME} ${SOURCES} ${C_SOURCES})

target_link_libraries(${PROJECT_NAME} ${ADDITIONAL_LIBS} ${LibsNamesList})