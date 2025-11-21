cmake_minimum_required(VERSION 3.26)
set(EXAMPLE_NAME 04-mesh-loading)

project(etu-opengl-example-${EXAMPLE_NAME})

file(GLOB_RECURSE CPP_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE GLSL_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.glsl)
file(GLOB_RECURSE ASSETS_FILES ${CMAKE_CURRENT_SOURCE_DIR}/assets/*)

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

if (ASSETS_FILES)
    add_custom_command(TARGET ${EXAMPLE_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory
                $<TARGET_FILE_DIR:${EXAMPLE_NAME}>/assets

        COMMAND ${CMAKE_COMMAND} -E copy 
                ${ASSETS_FILES}
                $<TARGET_FILE_DIR:${EXAMPLE_NAME}>/assets

        COMMENT "Copy all assets files in ${EXAMPLE_NAME}/assets"
    )
endif()
