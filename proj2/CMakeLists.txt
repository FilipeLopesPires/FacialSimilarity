project(proj2)

cmake_minimum_required(VERSION 2.8)

SET (CMAKE_CXX_STANDARD 17)

add_executable (wavhist
        src/wavhist.cpp
        src/headers/io.h
        src/io.cpp)
target_link_libraries (wavhist sndfile)

add_executable (wavquant
        src/wavquant.cpp
        src/headers/io.h
        src/io.cpp)
target_link_libraries (wavquant sndfile)

add_executable (wavcb
        src/wavcb.cpp
        src/headers/vctQuant.h
        src/vctQuant.cpp
        src/headers/io.h
        src/io.cpp)
target_link_libraries (wavcb sndfile pthread)

add_executable (wavfind
        src/wavfind.cpp
        src/headers/vctQuant.h
        src/vctQuant.cpp
        src/headers/io.h
        src/io.cpp)
target_link_libraries (wavfind sndfile)

add_executable (wavcmp
        src/wavcmp.cpp
        src/headers/vctQuant.h
        src/vctQuant.cpp
        src/headers/io.h
        src/io.cpp)
target_link_libraries (wavcmp sndfile)
