# Get the GLM environment variable. Since GLM is a header-only library, we
# just need to add it to the include directory.
set(GLM_INCLUDE_DIR "$ENV{GLM_INCLUDE_DIR}")
if (NOT GLM_INCLUDE_DIR)
   set(GLM_INCLUDE_DIR "${BACKUP_GLM}")
   if(NOT GLM_INCLUDE_DIR)
      MESSAGE(FATAL_ERROR "Please point the environment variable GLM_INCLUDE_DIR to the include directory of your GLM installation.")
    endif()
endif ()
include_directories(${GLM_INCLUDE_DIR})

# Get the GLFW environment variable. There should be a CMakeLists.txt in the 
# specified directory.
set(GLFW_DIR "$ENV{GLFW_DIR}")
if (NOT GLFW_DIR)
	set(GLFW_DIR "${BACKUP_GLFW}")
	if(NOT GLFW_DIR)
		message(FATAL_ERROR "Please point the environment variable GLFW_DIR to the root directory of your GLFW3 installation.")
	endif()
endif ()
option(GLFW_BUILD_EXAMPLES "GLFW_BUILD_EXAMPLES" OFF)
option(GLFW_BUILD_TESTS "GLFW_BUILD_TESTS" OFF)
option(GLFW_BUILD_DOCS "GLFW_BUILD_DOCS" OFF)
if (CMAKE_BUILD_TYPE MATCHES Release)
    add_subdirectory(${GLFW_DIR} ${GLFW_DIR}/release)
else ()
    add_subdirectory(${GLFW_DIR} ${GLFW_DIR}/debug)
endif ()
include_directories(${GLFW_DIR}/include)
