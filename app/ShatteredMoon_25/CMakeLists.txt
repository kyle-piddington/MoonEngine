project(shatteredMoon_25)
# Use glob to get the list of all source files.
file(GLOB_RECURSE SOURCES "src/*.cpp")

#Backup hard file locations of the link_libraries

# We don't really need to include header and resource files to build, but it's
# nice to have them show up in IDEs.
file(GLOB_RECURSE HEADERS "src/*.h")
file(GLOB_RECURSE GLSL "resources/*.vs" "resources/*.fs" "resources/*.gs")


find_package(assimp REQUIRED)
include_directories(${ASSIMP_INCLUDE_DIR})

# Set the executable.
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} ${GLSL})

#Link app library
target_link_libraries(${PROJECT_NAME} MoonEngine )
file(COPY resources DESTINATION ${PROJECT_BINARY_DIR})
file(COPY moonengine.cfg DESTINATION ${PROJECT_BINARY_DIR})



#Windows specific: Copy FMod DLLs to debug and release
MACRO(GetFModDLLs DEBUG_NAME RELEASE_NAME)
                string(REPLACE "_vc.lib" ".dll" FMOD_DLL
"${FMOD_LIBRARY_DEBUG}")
                set (${DEBUG_NAME} ${${DEBUG_NAME}} ${FMOD_DLL})
                string(REPLACE "_vc.lib" ".dll" FMOD_DLL
"${FMOD_LIBRARY}")
                set (${RELEASE_NAME} ${${RELEASE_NAME}} ${FMOD_DLL})
         

ENDMACRO()

if (WIN32)
        GetFModDLLs(DEBUG_DLLS RELEASE_DLLS)
      	set (DLL_TO_DBG ${CMAKE_CURRENT_BINARY_DIR}/Debug)
        set (DLL_TO_RELEASE ${CMAKE_CURRENT_BINARY_DIR}/Release)
        foreach(dll ${DEBUG_DLLS})
                file(COPY ${dll} DESTINATION ${DLL_TO_DBG})
        endforeach()
        foreach(dll ${RELEASE_DLLS})
                file(COPY ${dll} DESTINATION ${DLL_TO_RELEASE})
        endforeach()
endif(WIN32)

#Windows specific: Copy FMod DLLs to debug and release
MACRO(GetAssimpDLLs DEBUG_NAME RELEASE_NAME)
	string(REPLACE ".lib" ".dll" ASSIMP_DLL
			"${ASSIMP_LIBRARY}")
                set (${DEBUG_NAME} ${${DEBUG_NAME}} ${ASSIMP_DLL})
                set (${RELEASE_NAME} ${${RELEASE_NAME}} ${ASSIMP_DLL})
         

ENDMACRO()

if (WIN32)
	GetAssimpDLLs(DEBUG_DLLS RELEASE_DLLS)
      	set (DLL_TO_DBG ${CMAKE_CURRENT_BINARY_DIR}/Debug)
        set (DLL_TO_RELEASE ${CMAKE_CURRENT_BINARY_DIR}/Release)
        foreach(dll ${DEBUG_DLLS})
                file(COPY ${dll} DESTINATION ${DLL_TO_DBG})
        endforeach()
        foreach(dll ${RELEASE_DLLS})
                file(COPY ${dll} DESTINATION ${DLL_TO_RELEASE})
        endforeach()
endif(WIN32)
