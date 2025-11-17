cmake_minimum_required(VERSION 3.26)
project(etu-opengl-examples)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_FLAGS_DEBUG "-g -O -Wcpp -Wall -Wextra -Wpedantic" CACHE STRING "Flags for Debug" FORCE)
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -flto -march=native" CACHE STRING "Flags for Release" FORCE)
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g -ggdb" CACHE STRING "Flags for RelWithDebInfo" FORCE)
set(CMAKE_CXX_FLAGS_MINSIZEREL "-Os -DNDEBUG -flto" CACHE STRING "Flags for MinSizeRel" FORCE)

message(STATUS "Building examples")
set(EXAMPLES 
    01-hello-triangle
)

foreach (subdir ${EXAMPLES})
    set(SUBDIR_PATH ${CMAKE_CURRENT_SOURCE_DIR}/${subdir})
    file(GLOB_RECURSE CPP_SOURCES ${SUBDIR_PATH} *.cpp)

    add_executable(${subdir} ${CPP_SOURCES})
    target_link_libraries(${subdir} PUBLIC etu-opengl)
    target_include_directories(${subdir} PRIVATE ${SUBDIR_PATH}) 
endforeach()
