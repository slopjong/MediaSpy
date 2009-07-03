# -------------------------------------------------
# Project created by QtCreator 2009-06-26T13:39:09
# -------------------------------------------------
QT += sql
TARGET = mediaspy
TEMPLATE = app
UI_HEADERS_DIR = ui
UI_SOURCES_DIR = ui
MOC_DIR = src
RCC_DIR = src
OBJECTS_DIR = obj
SOURCES += src/main.cpp \
    src/mediaspy.cpp \
    src/collection.cpp \
    src/controller.cpp \
    src/databasemanager.cpp \
    src/collectiondialog.cpp \
    src/mediacollection.cpp
HEADERS += src/mediaspy.h \
    src/collection.h \
    src/controller.h \
    src/databasemanager.h \
    src/collectiondialog.h \
    src/mediacollection.h
FORMS += ui/mediaspy.ui \
    ui/collectiondialog.ui
RESOURCES += MediaSpy.qrc
OTHER_FILES += README.txt \
    LICENSE.txt \
    TODO.txt \
    CHANGELOG.txt

# define some useful values
PACKAGE_VERSION = 0.2
QMAKE_TARGET_PRODUCT = "MediaSpy"
QMAKE_TARGET_COPYRIGHT = "Copyright &copy; 2009 Stéphane Péchard"
win32:QMAKE_TARGET_COPYRIGHT = "Copyright (C) 2009 Stéphane Péchard"

# define variable for source code
DEFINES *= "PACKAGE_NAME=\"\\\"$${QMAKE_TARGET_PRODUCT}\\\"\"" \
    "PACKAGE_VERSION=\"\\\"$${PACKAGE_VERSION}\\\"\"" \
    "PACKAGE_COPYRIGHTS=\"\\\"$${QMAKE_TARGET_COPYRIGHT}\\\"\""
