cmake_minimum_required(VERSION 2.8)

find_package(OpenCV REQUIRED)

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -W")
SET(CMAKE_C_FLAGS_RELEASE "-O3")
SET(CMAKE_C_FLAGS_DEBUG "-g3")

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W -Wall -std=c++14")
SET(CMAKE_CXX_FLAGS_RELEASE "-O3")
SET(CMAKE_CXX_FLAGS_DEBUG "-g3")

SET(BASE_DIR ${CMAKE_SOURCE_DIR})
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BASE_DIR}/../bin)

INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIR})

add_executable(ImgCondComp ImgCondComp.cpp alphabet.cpp ctx.cpp fcm.cpp)
target_link_libraries(ImgCondComp ${OpenCV_LIBS})
