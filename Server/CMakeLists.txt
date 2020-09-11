cmake_minimum_required(VERSION 3.17)

project (server)

set (include_dir "${PROJECT_SOURCE_DIR}/include/")

include_directories (${include_dir})

set (source_dir "${PROJECT_SOURCE_DIR}/src/")

file (GLOB source_files "${source_dir}/*.c")

link_directories(/usr/lib64/)

add_executable (server ${source_files})

target_link_libraries(server sqlite3 pthread)