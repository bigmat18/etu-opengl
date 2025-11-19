cmake_minimum_required(VERSION 3.26)
set(EXAMPLE_NAME 01-hello-triangle)

project(etu-opengl-example-${EXAMPLE_NAME})
set(CMAKE_CXX_STANDARD 23)

file(GLOB_RECURSE CPP_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE GLSL_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.glsl)

add_executable(${EXAMPLE_NAME} ${CPP_SOURCES})
target_link_libraries(${EXAMPLE_NAME} PUBLIC etu-opengl)
target_include_directories(${EXAMPLE_NAME} PUBLIC ${SUBDIR_PATH}) 

if (GLSL_FILES)
    add_custom_command(TARGET ${EXAMPLE_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy 
                ${GLSL_FILES}
                $<TARGET_FILE_DIR:${EXAMPLE_NAME}>
        COMMENT "Copy all .glsl files in ${EXAMPLE_NAME}"
    )
endif()
