cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_STANDARD 11)

set(HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/midireader/smf.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/midireader/midi_chart.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/rgc/rgcwriter.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/rgc/rgcreader.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/binutils.hpp)

set(SOURCE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/midireader/smf.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/midireader/midi_chart.cpp
    #${CMAKE_CURRENT_SOURCE_DIR}/src/rgc/rgcwriter.cpp
    #${CMAKE_CURRENT_SOURCE_DIR}/src/rgc/rgcreader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/binutils.cpp)


include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src/midireader/
    ${CMAKE_CURRENT_SOURCE_DIR}/src/rgc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/
    )

add_executable(midi2rgc ${SOURCE})