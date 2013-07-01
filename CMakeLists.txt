cmake_minimum_required(VERSION 2.8)
project(spintax-permutations)

if(UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Werror")
endif()

add_subdirectory(src)
add_subdirectory(tests)
