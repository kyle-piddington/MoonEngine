cmake_minimum_required(VERSION 2.7)
project (ShatteredMoon)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
if(NOT WIN32)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
endif()
# Get the GLEW environment variable.
set(GLEW_DIR "$ENV{GLEW_DIR}")
if (NOT GLEW_DIR)
    message(FATAL_ERROR "Please point the environment variable GLEW_DIR to the root directory of your GLEW installation.")
endif ()
include_directories(${GLEW_DIR}/include)

set(GLM_DIR "$ENV{GLM_INCLUDE_DIR}")
if(NOT GLM_DIR)
	message(FATAL_ERROR "Please point the environment variable GLM_INCLUDE_DIR to the root of your GLM installation")
endif()
include_directories(${GLM_DIR})


include_directories(engine/src)


set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/deps )
message(${CMAKE_MODULE_PATH})
#FMod Support
find_package(FMOD)
if(FMOD_FOUND)
	include_directories(${FMOD_INCLUDE_DIRS})
endif()
add_subdirectory(deps)
add_subdirectory(engine)
add_subdirectory(app)
add_subdirectory(test)

#add_subdirectory(deps/assimp)
#include_directories(deps/assimp/include)


