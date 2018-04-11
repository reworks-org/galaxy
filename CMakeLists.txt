cmake_minimum_required(VERSION 3.11)
project(starlight)

# shoutout to stackoverflow, as alwayss
MACRO(SUBDIRLIST result curdir)
  FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
  SET(dirlist "")
  FOREACH(child ${children})
    IF(IS_DIRECTORY ${curdir}/${child})
      LIST(APPEND dirlist ${child})
    ENDIF()
  ENDFOREACH()
  SET(${result} ${dirlist})
ENDMACRO()

SUBDIRLIST(SUBDIRS "/src/sl/")

FOREACH(subdir ${SUBDIRS})
  include_directories(${subdir})
ENDFOREACH()

include_directories( ${MY_SOURCE_DIR}/src )

set(CMAKE_CXX_STANDARD 17)