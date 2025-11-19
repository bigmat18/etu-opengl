cmake_minimum_required(VERSION 3.26)
project(etu-opengl LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(EXTERNAL_DIR    ${CMAKE_CURRENT_SOURCE_DIR}/external)
set(INCLUDE_DIR     ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(SRC_DIR         ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(EXAMPLES_DIR    ${CMAKE_CURRENT_SOURCE_DIR}/examples)
set(GLAD_SOURCE_DIR ${EXTERNAL_DIR}/glad)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

add_library(glad STATIC ${GLAD_SOURCE_DIR}/src/glad.c)
target_include_directories(glad PUBLIC ${GLAD_SOURCE_DIR}/include)

option(CPPUTILS_BUILD_EXAMPLES "CPPUtils build examples" OFF)
option(CPPUTILS_USE_OPENMP "CPPUtils enable OpenMP" OFF)
option(CPPUTILS_ENABLE_WARNINGS "CPPUtils enable recommended warnings" OFF)
option(ETUGL_BUILD_EXAMPLES "ETUOpenGL build examples" ON)

add_subdirectory(${EXTERNAL_DIR}/cpp-utils-lib)
add_subdirectory(${EXTERNAL_DIR}/glm)

set(TARGET_LIB_NAME etu-opengl)
file(GLOB_RECURSE CPP_SOURCES ${SRC_DIR}/*.cpp)
add_library(${TARGET_LIB_NAME} ${CPP_SOURCES} ${EXTERNAL_DIR}/stb_image/stb_image_impl.cpp)
target_include_directories(${TARGET_LIB_NAME} PUBLIC ${INCLUDE_DIR})
target_include_directories(${TARGET_LIB_NAME} PUBLIC ${EXTERNAL_DIR}/stb_image)
target_link_libraries(${TARGET_LIB_NAME} PUBLIC 
    glad 
    glfw
    glm::glm
    cpp-utils-lib
    OpenGL::GL
)

if (ETUGL_BUILD_EXAMPLES)
    add_subdirectory(${EXAMPLES_DIR})
endif()

