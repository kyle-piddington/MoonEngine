project (MoonEngine)
# Use glob to get the list of all source files.
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE CCSOURCES "src/*.cc")

#Backup hard file locations of the link_libraries

# We don't really need to include header and resource files to build, but it's
# nice to have them show up in IDEs.
file(GLOB_RECURSE HEADERS "src/*.h")
file(GLOB_RECURSE GLSL "resources/*.glsl")

#expose headers in source to CMake
include_directories(HEADERS)

find_package(assimp)
if(assimp_FOUND)
    include_directories(${assimp_INCLUDE_DIRS})
endif()

# Set the executable.
add_library(MoonEngine ${SOURCES} ${CCSOURCES} ${HEADERS} ${GLSL} src/LevelEditor/Level.cpp src/LevelEditor/Level.h)

target_link_libraries(MoonEngine glfw ${assimp_LIBRARIES} ${FMOD_LIBRARIES} ${GLFW_LIBRARIES})


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

#Sets up Visual Studio Filters in the sln based on the folder structure
#NOTE: The cmake needs to be rerun/solution rebuilt if we add new files
#as the GLOB flag does not update when new files are added if an existing
#cache already exists 
function(VS_SOURCE_FILTERS)
    file(GLOB_RECURSE ALL_SOURCE "src/*.cpp" "src/*.h")
    foreach(_source IN ITEMS ${ALL_SOURCE})
        if (IS_ABSOLUTE "${_source}")
            file(RELATIVE_PATH _source_rel "${CMAKE_CURRENT_SOURCE_DIR}" "${_source}")
        else()
            set(source_rel "${_source}")
        endif()
        get_filename_component(_source_path "${_source_rel}" PATH)
        string(REPLACE "/" "\\" _source_path_msvc "${_source_path}")
        source_group("${_source_path_msvc}" FILES "${_source}")
    endforeach()
endfunction(VS_SOURCE_FILTERS)

# OS specific options and libraries
if (WIN32)
    # -Wall produces way too many warnings.
    # -pedantic is not supported.
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
   find_package(OpenGL REQUIRED)
   target_link_libraries(MoonEngine ${OPENGL_gl_LIBRARY})
   VS_SOURCE_FILTERS( )

else ()
    # Enable all pedantic warnings if not on WIN32.
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -pedantic -g")
    if (APPLE)
        # Add required frameworks for GLFW.
        target_link_libraries(MoonEngine "-framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo")
    else ()
        #Link the Linux OpenGL library
        target_link_libraries(MoonEngine "GL")
    endif ()
endif ()

