cmake_minimum_required(VERSION 2.8)


SET (CMAKE_CXX_STANDARD 17)
add_executable (wavcp src/wavcp.cpp)
target_link_libraries (wavcp sndfile)

add_executable (wavhist src/wavhist.cpp)
target_link_libraries (wavhist sndfile)

add_executable (wavquant src/wavquant.cpp)
target_link_libraries (wavquant sndfile)
