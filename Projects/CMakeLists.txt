if (WIN32)
    set(CMAKE_VER 3.20)
else()
    set(CMAKE_VER 3.10)
endif()

cmake_minimum_required(VERSION ${CMAKE_VER})

set(ADDITIONAL_LIBS "")
if(WIN32)
    set(CMAKE_CXX_COMPILER "C:/Program Files/LLVM/bin/clang++.exe")
    set(CMAKE_C_COMPILER "C:/Program Files/LLVM/bin/clang.exe")
    set(PROJECT_NAME ${PROJECT}-${BUILD_TYPE})
    set(ADDITIONAL_LIBS winmm.lib Version.lib Setupapi.lib)# libvcruntime.lib vcruntime.lib vcruntimed.lib msvcrtd.lib)
else()
    set(PROJECT_NAME ${PROJECT}-${BUILD_TYPE}.exe)
    set(ADDITIONAL_LIBS -ldl)
endif()
project(${PROJECT_NAME})


set(PARENT_DIR ${CMAKE_SOURCE_DIR}/..)
set(PROJECT_ROOT ${PARENT_DIR}/Projects/${PROJECT}/Sources)
set(COMMON_ROOT ${PARENT_DIR}/Common/Sources)

if(WIN32)

    add_subdirectory(${PARENT_DIR}/Common ${PARENT_DIR}/Builds/win/${PROJECT}/${BUILD_TYPE}/Engine)

    set (BuildDir "${PARENT_DIR}/Builds")
    add_library(SDL2 STATIC IMPORTED)
    set_target_properties(SDL2 PROPERTIES
        IMPORTED_LOCATION_RELEASE "${BuildDir}/win/libSDL2.a"
        IMPORTED_LOCATION_DEBUG "${BuildDir}/win/libSDL2.a"
        LINKER_LANGUAGE C)
    add_library(assimp STATIC IMPORTED)
    set_target_properties(assimp PROPERTIES
            IMPORTED_LOCATION_DEBUG "${BuildDir}/win/assimp-vc142-mt.lib"
            IMPORTED_LOCATION_RELEASE "${BuildDir}/win/assimp-vc142-mt.lib"
            LINKER_LANGUAGE C)
else()
    add_subdirectory(${PARENT_DIR}/Common ${PARENT_DIR}/Builds/unix/${PROJECT}/${BUILD_TYPE}/Engine)
    find_package(SDL2 REQUIRED)
    include_directories(${SDL2_INCLUDE_DIRS})
    find_package(assimp REQUIRED)
endif()


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-pragma-pack -Wno-deprecated-declarations")


if(BUILD_TYPE STREQUAL "dbg")
    if (WIN32)
        set(SANITIZER, "-fsanitize=address -fsanitize-address-use-after-scope -fno-omit-frame-pointer -fsanitize=undefined")
    else()
        set(SANITIZER, "-fsanitize=address -fsanitize-address-use-after-scope -fno-omit-frame-pointer -fsanitize=leak -fsanitize=undefined -fsanitize=bounds-strict")
    endif()

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -g -O0 -DDEBUG ${SANITIZER}")
    set(CMAKE_BUILD_TYPE "Debug")
elseif(BUILD_TYPE STREQUAL "dev")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -DDEBUG")
    set(CMAKE_BUILD_TYPE "Debug")
elseif(BUILD_TYPE STREQUAL "rel")
    if (WIN32)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx2 -O3 -fopenmp")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx2 -O3")
    endif()
    set(CMAKE_BUILD_TYPE  "Release")
    message("Release Build ${CMAKE_CXX_FLAGS}")
endif()

include_directories(${COMMON_ROOT})
include_directories(${PROJECT_ROOT})
file(GLOB_RECURSE PROJECT_SOURCES  ${PROJECT_ROOT}/*.cpp)
file(GLOB_RECURSE PROJECT_C_SOURCES ${PROJECT_ROOT}/*.c)

if (DLL STREQUAL "yes")
    #include(GNUInstallDirs)
    add_library(${PROJECT_NAME} SHARED ${PROJECT_SOURCES} ${PROJECT_C_SOURCES})
    target_link_libraries(${PROJECT_NAME} SDL2 assimp Engine-${BUILD_TYPE} ${ADDITIONAL_LIBS})
else()
    add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_C_SOURCES})
    target_link_libraries(${PROJECT_NAME} SDL2 assimp Engine-${BUILD_TYPE} ${ADDITIONAL_LIBS})
endif()



