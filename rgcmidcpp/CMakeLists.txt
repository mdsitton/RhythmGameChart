cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

# CMake on windows doesnt seem to support setting up for C++17 yet.
if(MSVC)
    add_compile_options("/std:c++latest")
endif()


set(HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/midi/smf.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/midi/midi_chart.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/rgc/rgcwriter.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/rgc/rgcreader.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/rgc/rgctypes.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/chart/chartreader.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/timer.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/binutils.hpp)

set(SOURCE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/midi/smf.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/midi/midi_chart.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/rgc/rgcwriter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/rgc/rgcreader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/chart/chartreader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/timer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/binutils.cpp)


include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src/chart/
    ${CMAKE_CURRENT_SOURCE_DIR}/src/midi/
    ${CMAKE_CURRENT_SOURCE_DIR}/src/rgc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/
    )

add_executable(midi2rgc ${SOURCE})