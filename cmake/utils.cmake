function(copy_resources target_name resource_root_dir)
    set(target_output_dir $<TARGET_FILE_DIR:${target_name}>)

    add_custom_command(TARGET ${target_name} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${resource_root_dir}
                ${target_output_dir}

        COMMAND ${CMAKE_COMMAND} -E rm -f 
                ${target_output_dir}/*.cpp 
                ${target_output_dir}/*.cxx 
                ${target_output_dir}/*.cc 
                ${target_output_dir}/*.c
                ${target_output_dir}/*.hpp 
                ${target_output_dir}/*.h 
                ${target_output_dir}/*.in
                ${target_output_dir}/CMakeLists.txt

        COMMENT "Copying resources of ${target_name}"
        VERBATIM
    )
endfunction()
