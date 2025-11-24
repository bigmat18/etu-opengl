cmake_minimum_required(VERSION 3.26)
set(EXAMPLE_NAME 05-materials)

project(etu-opengl-example-${EXAMPLE_NAME})
set(CMAKE_CXX_STANDARD 23)

file(GLOB_RECURSE CPP_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
add_executable(${EXAMPLE_NAME} ${CPP_SOURCES})
target_link_libraries(${EXAMPLE_NAME} PUBLIC etu-opengl)
target_include_directories(${EXAMPLE_NAME} PUBLIC ${SUBDIR_PATH}) 


include(utils)
copy_resources(${EXAMPLE_NAME} ${CMAKE_CURRENT_SOURCE_DIR})
link_shaders(${EXAMPLE_NAME})
