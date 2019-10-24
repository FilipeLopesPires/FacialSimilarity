project(proj2)

cmake_minimum_required(VERSION 2.8)

SET (CMAKE_CXX_STANDARD 17)

add_executable (wavcp src/wavcp.cpp)
target_link_libraries (wavcp sndfile)

add_executable (wavhist src/wavhist.cpp)
target_link_libraries (wavhist sndfile)

add_executable (wavquant src/wavquant.cpp)
target_link_libraries (wavquant sndfile)

add_executable (wavcb src/wavcb.cpp)
target_link_libraries (wavcb sndfile)

add_executable (wavcmp src/wavcmp.cpp src/headers/snr.h src/snr.cpp src/headers/io.h src/io.cpp)
target_link_libraries (wavcmp sndfile)
