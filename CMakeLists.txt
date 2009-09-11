#cmake configuration version if not found
cmake_minimum_required(VERSION 2.6)

#project name
project(mediaspy)

#version of the program for release ready states
set(MEDIASPY_VERSION_MAJOR 0)
set(MEDIASPY_VERSION_MINOR 7)
set(MEDIASPY_VERSION_PATCH 2)
set(MEDIASPY_VERSION "${MEDIASPY_VERSION_MAJOR}.${MEDIASPY_VERSION_MINOR}.${MEDIASPY_VERSION_PATCH}")

# define some useful values
# set(ENV{PACKAGE_COPYRIGHTS} "Copyright &copy; 2009 Stéphane Péchard") # NOK
# set(ENV{PACKAGE_NAME} "MediaSpy")
# set(ENV{PACKAGE_VERSION} ${MEDIASPY_VERSION})

# build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# what is it for?
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# finds
# find_package(KDE4 REQUIRED) # NOK
find_package(Qt4 4.5
  COMPONENTS QtCore QtGui QtWebKit QtNetwork QtSql)

# some debug def
add_definitions(-Wall -fexceptions)

# includes
include( ${QT_USE_FILE} )
include_directories(${KDE4_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

# where the code is
add_subdirectory(src)

