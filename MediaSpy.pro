# -------------------------------------------------
# Project created by QtCreator 2009-06-26T13:39:09
# -------------------------------------------------
QT += sql \
    webkit \
    network
TARGET = mediaspy
TEMPLATE = app
UI_DIR = build/ui
MOC_DIR = build/moc
RCC_DIR = build/rcc
OBJECTS_DIR = build
SOURCES += src/main.cpp \
    src/mediaspy.cpp \
    src/collection.cpp \
    src/collectiondialog.cpp \
    src/databasemanager.cpp \
    src/updatethread.cpp \
    src/info/imdbthread.cpp \
    src/info/infomanager.cpp \
    src/info/infoimdb.cpp \
    src/info/infosupplier.cpp \
    src/media/mediacollection.cpp \
    src/media/media.cpp \
    src/media/moviemedia.cpp \
    src/qtElements/myqlineedit.cpp \
    src/qtElements/myqsqltablemodel.cpp \
    src/qtElements/myqwebview.cpp \
    src/qtElements/myqsortfilterproxymodel.cpp \
    src/qtElements/myqlistview.cpp
HEADERS += src/mediaspy.h \
    src/collection.h \
    src/collectiondialog.h \
    src/databasemanager.h \
    src/updatethread.h \
    src/info/imdbthread.h \
    src/info/infomanager.h \
    src/info/infoimdb.h \
    src/info/infosupplier.h \
    src/media/mediacollection.h \
    src/media/media.h \
    src/media/moviemedia.h \
    src/qtElements/myqlineedit.h \
    src/qtElements/myqsqltablemodel.h \
    src/qtElements/myqwebview.h \
    src/qtElements/myqsortfilterproxymodel.h \
    src/qtElements/myqlistview.h
FORMS += src/ui/mediaspy.ui \
    src/ui/collectiondialog.ui
RESOURCES += src/resources/MediaSpy.qrc
TRANSLATIONS += translations/mediaspy_fr.ts \
    translations/mediaspy_ta.ts
OTHER_FILES += README.txt \
    LICENSE.txt \
    TODO.txt \
    CHANGELOG.txt

# define some useful values
PACKAGE_VERSION = 0.6.4-testing
QMAKE_TARGET_PRODUCT = "MediaSpy"
QMAKE_TARGET_COPYRIGHT = "Copyright &copy; 2009 Stéphane Péchard"
win32:QMAKE_TARGET_COPYRIGHT = "Copyright (C) 2009 Stéphane Péchard"

# define variable for source code
DEFINES *= "PACKAGE_NAME=\"\\\"$${QMAKE_TARGET_PRODUCT}\\\"\"" \
    "PACKAGE_VERSION=\"\\\"$${PACKAGE_VERSION}\\\"\"" \
    "PACKAGE_COPYRIGHTS=\"\\\"$${QMAKE_TARGET_COPYRIGHT}\\\"\"" \
    "MEDIASPY_HOME=\"$(MEDIASPY_HOME)\""
