cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_STANDARD 11)

set(rgc_source_dir "${CMAKE_CURRENT_SOURCE_DIR}/Rhythm Game Chart Format cpp")

# Not really needed but its just a habit of mine to also defining the headers here...
set(HEADERS
    "${rgc_source_dir}/BaseIO.h"
    "${rgc_source_dir}/IO Util.h"
    "${rgc_source_dir}/Reader.h"
    "${rgc_source_dir}/Writer.h")

set(SOURCE
    "${rgc_source_dir}/Rhythm Game Chart Format.cpp"
    "${rgc_source_dir}/BaseIO.cpp"
    "${rgc_source_dir}/IO Util.cpp"
    "${rgc_source_dir}/Reader.cpp"
    "${rgc_source_dir}/Writer.cpp")


if(CMAKE_SYSTEM_NAME MATCHES "Windows")
    set(SOURCE $SOURCE
        "${rgc_source_dir}/stdafx.cpp")
endif()

include_directories(${rgc_source_dir})

add_executable(rgcf ${SOURCE})