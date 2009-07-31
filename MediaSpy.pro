# -------------------------------------------------
# Project created by QtCreator 2009-06-26T13:39:09
# -------------------------------------------------
QT += sql \
    webkit \
    network
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
    src/databasemanager.cpp \
    src/collectiondialog.cpp \
    src/mediacollection.cpp \
    src/media.cpp \
    src/updatethread.cpp \
    src/mediafilter.cpp \
    src/infoview.cpp \
    src/infomanager.cpp \
    src/infoimdb.cpp \
    src/moviemedia.cpp \
    src/infosupplier.cpp \
    src/myqsqltablemodel.cpp
HEADERS += src/mediaspy.h \
    src/collection.h \
    src/databasemanager.h \
    src/collectiondialog.h \
    src/mediacollection.h \
    src/media.h \
    src/updatethread.h \
    src/mediafilter.h \
    src/infoview.h \
    src/infomanager.h \
    src/infoimdb.h \
    src/moviemedia.h \
    src/infosupplier.h \
    src/myqsqltablemodel.h
FORMS += ui/mediaspy.ui \
    ui/collectiondialog.ui
RESOURCES += src/resources/MediaSpy.qrc
TRANSLATIONS += translations/mediaspy_fr.ts \
    translations/mediaspy_ta.ts
OTHER_FILES += README.txt \
    LICENSE.txt \
    TODO.txt \
    CHANGELOG.txt

# define some useful values
PACKAGE_VERSION = 0.6
QMAKE_TARGET_PRODUCT = "MediaSpy"
QMAKE_TARGET_COPYRIGHT = "Copyright &copy; 2009 Stéphane Péchard"
win32:QMAKE_TARGET_COPYRIGHT = "Copyright (C) 2009 Stéphane Péchard"

# define variable for source code
DEFINES *= "PACKAGE_NAME=\"\\\"$${QMAKE_TARGET_PRODUCT}\\\"\"" \
    "PACKAGE_VERSION=\"\\\"$${PACKAGE_VERSION}\\\"\"" \
    "PACKAGE_COPYRIGHTS=\"\\\"$${QMAKE_TARGET_COPYRIGHT}\\\"\"" \
    "MEDIASPY_HOME=\"$(MEDIASPY_HOME)\""
