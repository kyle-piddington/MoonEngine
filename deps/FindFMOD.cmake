# - Find fmod
# Find the fmod includes and library
#
#  FMOD_INCLUDE_DIR - Where to find fmod includes
#  FMOD_LIBRARIES   - List of libraries when using fmod
#  FMOD_FOUND       - True if fmod was found

IF(FMOD_INCLUDE_DIR)
  SET(FMOD_FIND_QUIETLY TRUE)
ENDIF(FMOD_INCLUDE_DIR)

FIND_PATH(FMOD_INCLUDE_DIR "fmod.h"
  PATHS
  $ENV{FMOD_HOME}/inc
  $ENV{EXTERNLIBS}/fmod/inc
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local
  /usr
  /sw # Fink
  /opt/local # DarwinPorts
  /opt/csw # Blastwave
  /opt
  DOC "fmod - Headers"
)
if(MSVC)
SET(FMOD_NAMES fmod_vc)
SET(FMOD_DBG_NAMES fmodL_vc)
else(MSVC)
SET(FMOD_NAMES fmod)
SET(FMOD_DBG_NAMES fmodL)
endif(MSVC)

FIND_LIBRARY(FMOD_LIBRARY NAMES ${FMOD_NAMES}
  PATHS
  $ENV{FMOD_HOME}
  $ENV{EXTERNLIBS}/fmod
  $ENV{EXTERNLIBS}/fmod
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local
  /usr
  /sw
  /opt/local
  /opt/csw
  /opt
  PATH_SUFFIXES lib lib64
  DOC "fmod - Library"
)



INCLUDE(FindPackageHandleStandardArgs)

IF(MSVC)
  # VisualStudio needs a debug version
  FIND_LIBRARY(FMOD_LIBRARY_DEBUG NAMES ${FMOD_DBG_NAMES}
    PATHS
    $ENV{FMOD_HOME}
    $ENV{EXTERNLIBS}/fmod
    PATH_SUFFIXES lib lib64
    DOC "fmod - Library (Debug)"
  )
  IF(FMOD_LIBRARY_DEBUG AND FMOD_LIBRARY)
    SET(FMOD_LIBRARIES optimized ${FMOD_LIBRARY} debug ${FMOD_LIBRARY_DEBUG})
  ENDIF(FMOD_LIBRARY_DEBUG AND FMOD_LIBRARY)

  FIND_PACKAGE_HANDLE_STANDARD_ARGS(FMOD DEFAULT_MSG FMOD_LIBRARY FMOD_LIBRARY_DEBUG FMOD_INCLUDE_DIR)

  MARK_AS_ADVANCED(FMOD_LIBRARY FMOD_LIBRARY_DEBUG FMOD_INCLUDE_DIR)
  
ELSE(MSVC)
  # rest of the world
  SET(FMOD_LIBRARIES ${FMOD_LIBRARY} )

  FIND_PACKAGE_HANDLE_STANDARD_ARGS(FMOD DEFAULT_MSG FMOD_LIBRARY FMOD_INCLUDE_DIR)
  
  MARK_AS_ADVANCED(FMOD_LIBRARY FMOD_INCLUDE_DIR)
  
ENDIF(MSVC)

IF(FMOD_FOUND)
  SET(FMOD_INCLUDE_DIRS ${FMOD_INCLUDE_DIR})
ENDIF(FMOD_FOUND)
