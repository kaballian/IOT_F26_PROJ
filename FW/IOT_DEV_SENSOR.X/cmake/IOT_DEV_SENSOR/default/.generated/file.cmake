# The following variables contains the files used by the different stages of the build process.
set(IOT_DEV_SENSOR_default_default_XC8_FILE_TYPE_assemble)
set_source_files_properties(${IOT_DEV_SENSOR_default_default_XC8_FILE_TYPE_assemble} PROPERTIES LANGUAGE ASM)

# For assembly files, add "." to the include path for each file so that .include with a relative path works
foreach(source_file ${IOT_DEV_SENSOR_default_default_XC8_FILE_TYPE_assemble})
        set_source_files_properties(${source_file} PROPERTIES INCLUDE_DIRECTORIES "$<PATH:NORMAL_PATH,$<PATH:REMOVE_FILENAME,${source_file}>>")
endforeach()

set(IOT_DEV_SENSOR_default_default_XC8_FILE_TYPE_assemblePreprocess)
set_source_files_properties(${IOT_DEV_SENSOR_default_default_XC8_FILE_TYPE_assemblePreprocess} PROPERTIES LANGUAGE ASM)

# For assembly files, add "." to the include path for each file so that .include with a relative path works
foreach(source_file ${IOT_DEV_SENSOR_default_default_XC8_FILE_TYPE_assemblePreprocess})
        set_source_files_properties(${source_file} PROPERTIES INCLUDE_DIRECTORIES "$<PATH:NORMAL_PATH,$<PATH:REMOVE_FILENAME,${source_file}>>")
endforeach()

set(IOT_DEV_SENSOR_default_default_XC8_FILE_TYPE_compile
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../src/FAN.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../src/I2C.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../src/PWM.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../src/main.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../src/system.c")
set_source_files_properties(${IOT_DEV_SENSOR_default_default_XC8_FILE_TYPE_compile} PROPERTIES LANGUAGE C)
set(IOT_DEV_SENSOR_default_default_XC8_FILE_TYPE_link)
set(IOT_DEV_SENSOR_default_image_name "default.elf")
set(IOT_DEV_SENSOR_default_image_base_name "default")

# The output directory of the final image.
set(IOT_DEV_SENSOR_default_output_dir "${CMAKE_CURRENT_SOURCE_DIR}/../../../out/IOT_DEV_SENSOR")

# The full path to the final image.
set(IOT_DEV_SENSOR_default_full_path_to_image ${IOT_DEV_SENSOR_default_output_dir}/${IOT_DEV_SENSOR_default_image_name})

# Potential output file extensions
set(output_extensions
    .hex
    .hxl
    .mum
    .o
    .sdb
    .sym
    .cmf)
list(TRANSFORM output_extensions PREPEND "${IOT_DEV_SENSOR_default_output_dir}/${IOT_DEV_SENSOR_default_image_base_name}")
