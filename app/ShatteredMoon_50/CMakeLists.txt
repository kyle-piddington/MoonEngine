project(shatteredMoon_50)
# Use glob to get the list of all source files.
file(GLOB_RECURSE SOURCES "src/*.cpp")

#Backup hard file locations of the link_libraries

# We don't really need to include header and resource files to build, but it's
# nice to have them show up in IDEs.
file(GLOB_RECURSE HEADERS "src/*.h")
file(GLOB_RECURSE GLSL "resources/*.vs" "resources/*.fs" "resources/*.gs")

# Set the executable.
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} ${GLSL})

#Link app library
target_link_libraries(${PROJECT_NAME} MoonEngine)
file(COPY resources DESTINATION ${PROJECT_BINARY_DIR})
file(COPY moonengine.cfg DESTINATION ${PROJECT_BINARY_DIR})
