project (UnitTests)


 
# Use glob to get the list of all source files.
file(GLOB_RECURSE SOURCES "test/*.cpp")

#Backup hard file locations of the link_libraries

# We don't really need to include header and resource files to build, but it's
# nice to have them show up in IDEs.
file(GLOB_RECURSE HEADERS "test/*.h" "test/*.hpp")
file(GLOB_RECURSE GLSL "resources/*.glsl")

# Set the executable.
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} ${GLSL})

#Link testing app library
target_link_libraries(${PROJECT_NAME} MoonEngine)
