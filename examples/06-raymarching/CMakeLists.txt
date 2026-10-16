cmake_minimum_required(VERSION 3.28)
set(EXAMPLE_NAME 06-raymarching)

project(etu-opengl-example-${EXAMPLE_NAME})
set(CMAKE_CXX_STANDARD 26)

find_package(nlohmann_json REQUIRED)

file(GLOB_RECURSE CPP_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
add_executable(${EXAMPLE_NAME} ${CPP_SOURCES})
target_link_libraries(${EXAMPLE_NAME} PUBLIC etu-opengl nlohmann_json::nlohmann_json)
target_include_directories(${EXAMPLE_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}) 

include(utils)
copy_resources(${EXAMPLE_NAME} ${CMAKE_CURRENT_SOURCE_DIR})
link_shaders(${EXAMPLE_NAME})
