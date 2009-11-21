set(CMAKE_BUILD_TYPE "Release")

#project name
project(mediaspy)

#version of the program for release ready states
set(MEDIASPY_VERSION_MAJOR 0)
set(MEDIASPY_VERSION_MINOR 7)
set(MEDIASPY_VERSION_PATCH 4)
set(MEDIASPY_VERSION "${MEDIASPY_VERSION_MAJOR}.${MEDIASPY_VERSION_MINOR}.${MEDIASPY_VERSION_PATCH}")

# some debug def
add_definitions(-fexceptions)

# finds
find_package(KDE4 REQUIRED) # NOK
include(KDE4Defaults)
include(MacroLibrary)

# CPack definitions
add_definitions(-DPACKAGE_COPYRIGHTS="\\"Copyright - 2009 Stéphane Péchard\\"")
add_definitions(-DPACKAGE_NAME="\\"MediaSpy\\"")
add_definitions(-DPACKAGE_VERSION="\\"${MEDIASPY_VERSION}\\"")

# includes
include_directories(${KDE4_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

# where the code is
add_subdirectory(src)

# for something
macro_display_feature_log()

