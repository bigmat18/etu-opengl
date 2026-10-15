cmake_minimum_required(VERSION 3.28)
project(etu-opengl LANGUAGES C CXX VERSION 1.0.0)

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(EXTERNAL_DIR    ${CMAKE_CURRENT_SOURCE_DIR}/external)
set(INCLUDE_DIR     ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(SRC_DIR         ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(EXAMPLES_DIR    ${CMAKE_CURRENT_SOURCE_DIR}/examples)
if(NOT DEFINED GLAD_SOURCE_DIR)
    set(GLAD_SOURCE_DIR ${EXTERNAL_DIR}/glad)
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(tinyobjloader REQUIRED)
find_package(cpp-core-utils CONFIG REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(STB REQUIRED IMPORTED_TARGET stb)

add_library(glad STATIC ${GLAD_SOURCE_DIR}/src/glad.c)
target_include_directories(glad PUBLIC 
    $<BUILD_INTERFACE:${GLAD_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

option(ETUGL_BUILD_EXAMPLES "ETUOpenGL build examples" ON)

set(TARGET_LIB_NAME etu-opengl)
file(GLOB_RECURSE CPP_SOURCES ${SRC_DIR}/*.cpp)

add_library(${TARGET_LIB_NAME} ${CPP_SOURCES})
target_include_directories(${TARGET_LIB_NAME} PUBLIC 
    $<BUILD_INTERFACE:${INCLUDE_DIR}>
    $<BUILD_INTERFACE:${GLAD_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_link_libraries(${TARGET_LIB_NAME} PUBLIC 
    glad 
    glfw
    glm::glm
    cpp-core-utils::cpp-core-utils
    OpenGL::GL
    tinyobjloader::tinyobjloader
    PkgConfig::STB
)

install(TARGETS ${TARGET_LIB_NAME} glad
    EXPORT etu-openglTargets
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY ${INCLUDE_DIR}/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING PATTERN "*.hpp" PATTERN "*.h"
)

install(DIRECTORY ${GLAD_SOURCE_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING PATTERN "*.hpp" PATTERN "*.h"
)

install(EXPORT etu-openglTargets
    FILE etu-openglTargets.cmake
    NAMESPACE etu-opengl::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/etu-opengl
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/etu-openglConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/etu-openglConfig.cmake"
    "include(CMakeFindDependencyMacro)
     find_dependency(OpenGL)
     find_dependency(glfw3)
     find_dependency(glm)
     find_dependency(tinyobjloader)
     find_dependency(cpp-core-utils)
     include(\"\${CMAKE_CURRENT_LIST_DIR}/etu-openglTargets.cmake\")"
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/etu-openglConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/etu-openglConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/etu-opengl
)

if (ETUGL_BUILD_EXAMPLES)
    add_subdirectory(${EXAMPLES_DIR})
endif()
