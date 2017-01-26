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

# Set the executable.
add_library(MoonEngine ${SOURCES} ${CCSOURCES} ${HEADERS} ${GLSL})


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


function(_SETUP)
	file(GLOB_RECURSE SRC_COLLISION		"src/Collision/*.cpp" 	"src/Collision/*.h")
	file(GLOB_RECURSE SRC_COMPONENT 	"src/Component/*.cpp" 	"src/Component/*.h")
	file(GLOB_RECURSE SRC_ENGINEAPP 	"src/EngineApp/*.cpp" 	"src/EngineApp/*.h")
    file(GLOB_RECURSE SRC_GAMEOBJECT 	"src/GameObject/*.cpp"  "src/GameObject/*.h")
    file(GLOB_RECURSE SRC_GEOMETRY	    "src/Geometry/*.cpp"    "src/Geometry/*.h")
    file(GLOB_RECURSE SRC_GL 		    "src/GL/*.cpp"		    "src/GL/*.h")
    file(GLOB_RECURSE SRC_GLOBALFUNCS   "src/GlobalFuncs/*.cpp"	"src/GlobalFuncs/*.h")
    file(GLOB_RECURSE SRC_GLUTIL 	    "src/GLUtil/*.cpp"      "src/GLUtil/*.h")
    file(GLOB_RECURSE SRC_IO 		    "src/IO/*.cpp"		    "src/IO/*.h")
    file(GLOB_RECURSE SRC_LOADERS       "src/Loaders/*.cpp"     "src/Loaders/*.h")
    file(GLOB_RECURSE SRC_RENDER	    "src/Render/*.cpp"      "src/Render/*.h")
    file(GLOB_RECURSE SRC_SCENE		    "src/Scene/*.cpp"	    "src/Scene/*.h")
    file(GLOB_RECURSE SRC_THIRDPARTY    "src/ThirdParty/*.cpp"	"src/ThirdParty/*.cc"	"src/ThirdParty/*.h")
    file(GLOB_RECURSE SRC_UTIL 		    "src/Util/*.cpp"	    "src/Util/*.h")

	source_group("Collision"		FILES ${SRC_COLLISION})
	source_group("Component"    	FILES ${SRC_COMPONENT})
	source_group("EngineApp"    	FILES ${SRC_ENGINEAPP})
	source_group("GameObject"   	FILES ${SRC_GAMEOBJECT})
	source_group("Geometry"    		FILES ${SRC_GEOMETRY})
	source_group("GL"    			FILES ${SRC_GL})
	source_group("GlobalFuncs"		FILES ${SRC_GLOBALFUNCS})
	source_group("GLUtil"    		FILES ${SRC_GLUTIL})
	source_group("IO"    			FILES ${SRC_IO})
	source_group("Loaders"    		FILES ${SRC_LOADERS})
	source_group("Render"    		FILES ${SRC_RENDER})
	source_group("Scene"    		FILES ${SRC_SCENE})
	source_group("ThirdParty" 		FILES ${SRC_THIRDPARTY})
	source_group("Util"    			FILES ${SRC_UTIL})
endfunction()

# OS specific options and libraries
if (WIN32)
    # -Wall produces way too many warnings.
    # -pedantic is not supported.
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
   find_package(OpenGL REQUIRED)
   target_link_libraries(MoonEngine ${OPENGL_gl_LIBRARY})
   _setup( )

else ()
    # Enable all pedantic warnings if not on WIN32.
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -pedantic")
    if (APPLE)
        # Add required frameworks for GLFW.
        target_link_libraries(MoonEngine "-framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo")
    else ()
        #Link the Linux OpenGL library
        target_link_libraries(MoonEngine "GL")
    endif ()
endif ()

