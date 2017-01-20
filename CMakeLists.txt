cmake_minimum_required(VERSION 2.7)
project (ShatteredMoon)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -Wall -pedantic")
   

include_directories(engine/src)

add_subdirectory(deps)
add_subdirectory(engine)
add_subdirectory(app)
add_subdirectory(test)
