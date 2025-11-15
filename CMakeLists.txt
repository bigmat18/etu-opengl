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

find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)

add_library(glad STATIC ${GLAD_SOURCE_DIR}/src/glad.c)
target_include_directories(glad PUBLIC ${GLAD_SOURCE_DIR}/include)
target_link_libraries(glad PUBLIC)

# ========== CPPUtils setup ===========
option(CPPUTILS_BUILD_EXAMPLES "CPPUtils build examples" OFF)
option(CPPUTILS_USE_OPENMP "CPPUtils enable OpenMP" OFF)
option(CPPUTILS_ENABLE_WARNINGS "CPPUtils enable recommended warnings" OFF)

add_subdirectory(${EXTERNAL_DIR}/cpp-utils-lib)
# ========== CPPUtils setup =========== 

set(TARGET_LIB_NAME etu-opengl)
#add_library(${TARGET_LIB_NAME})



