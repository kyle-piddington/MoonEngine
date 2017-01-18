project (MoonEngine)
# Use glob to get the list of all source files.
file(GLOB_RECURSE SOURCES "src/*.cpp")

#Backup hard file locations of the link_libraries

# We don't really need to include header and resource files to build, but it's
# nice to have them show up in IDEs.
file(GLOB_RECURSE HEADERS "src/*.h")
file(GLOB_RECURSE GLSL "resources/*.glsl")

#expose headers in source to CMake
include_directories("src")

# Set the executable.
add_library(MoonEngine ${SOURCES} ${HEADERS} ${GLSL})


target_link_libraries(MoonEngine glfw ${GLFW_LIBRARIES})


set(GLM_DIR "$ENV{GLM_INCLUDE_DIR}")
if(NOT GLM_DIR)
	message(FATAL_ERROR "Please point the environment variable GLM_INCLUDE_DIR to the root of your GLM installation")
endif()
include_directories(${GLM_DIR})

# Get the GLEW environment variable.
set(GLEW_DIR "$ENV{GLEW_DIR}")
if (NOT GLEW_DIR)
    message(FATAL_ERROR "Please point the environment variable GLEW_DIR to the root directory of your GLEW installation.")
endif ()
include_directories(${GLEW_DIR}/include)
if (WIN32)
    # With prebuilt binaries
    target_link_libraries(MoonEngine ${GLEW_DIR}/lib/Release/Win32/glew32s.lib)
else ()
    target_link_libraries(MoonEngine ${GLEW_DIR}/lib/libGLEW.a)
endif ()

# OS specific options and libraries
if (WIN32)
    # c++0x is enabled by default.
    # -Wall produces way too many warnings.
    # -pedantic is not supported.
   find_package(OpenGL REQUIRED)
   target_link_libraries(MoonEngine ${OPENGL_gl_LIBRARY})
else ()
    # Enable all pedantic warnings.
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -Wall -pedantic")
    if (APPLE)
        # Add required frameworks for GLFW.
        target_link_libraries(MoonEngine "-framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo")
    else ()
        #Link the Linux OpenGL library
        target_link_libraries(MoonEngine "GL")
    endif ()
endif ()




