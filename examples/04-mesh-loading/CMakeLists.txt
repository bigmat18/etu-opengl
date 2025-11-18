cmake_minimum_required(VERSION 3.26)
set(EXE_NAME 04-mesh-loading)

project(etu-opengl-example-${EXE_NAME})

file(GLOB_RECURSE CPP_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE GLSL_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.glsl)

add_executable(${EXE_NAME} ${CPP_SOURCES})
target_link_libraries(${EXE_NAME} PUBLIC etu-opengl)
target_include_directories(${EXE_NAME} PUBLIC ${SUBDIR_PATH}) 

add_custom_command(TARGET ${EXE_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${GLSL_FILES}
            $<TARGET_FILE_DIR:${EXE_NAME}>
    COMMENT "Copy of all .glsl files in ${EXE_NAME}"
)
