
include_directories(${KDE4_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

set(SOURCES
    collection.cpp
    collectiondialog.cpp
    databasemanager.cpp
    editmediadialog.cpp
    main.cpp
    mediaspy.cpp
    options.cpp
    optionsdialog.cpp
    updatethread.cpp
    info/imdbthread.cpp
    info/infoimdb.cpp
    info/infomanager.cpp
    info/infosupplier.cpp
    media/mediacollection.cpp
    media/media.cpp
    media/moviemedia.cpp
    qtElements/myqlineedit.cpp
    qtElements/myqlistview.cpp
    qtElements/myqsortfilterproxymodel.cpp
    qtElements/myqsqltablemodel.cpp
)

set(HEADERS
    collection.h
    collectiondialog.h
    editmediadialog.h
    mediaspy.h
    options.h
    optionsdialog.h
    updatethread.h
    info/imdbthread.h
    info/infoimdb.h
    info/infomanager.h
    info/infosupplier.h
    media/mediacollection.h
    qtElements/myqlineedit.h
    qtElements/myqlistview.h
    qtElements/myqsortfilterproxymodel.h
    qtElements/myqsqltablemodel.h
)

set(UI
    ui/collectiondialog.ui
    ui/editmediadialog.ui
    ui/mediaspy.ui
    ui/optionsdialog.ui
)

SET(RESOURCES
    resources/MediaSpy.qrc
)

# this will run uic on .ui files:
qt4_wrap_ui( UI_H ${UI} )

# and this will run moc:
qt4_wrap_cpp( MOC ${HEADERS} )

# this will generate rules that will run rcc on all resource files:
qt4_add_resources( RCS ${RESOURCES} )

# create an exe
add_executable( mediaspy ${SOURCES} ${RCS} ${MOC} ${UI_H} ) # OK
# kde4_add_executable( mediaspy ${SOURCES} ${RCS} ${MOC} ${UI_H} ) # NOK

target_link_libraries( mediaspy ${QT_LIBRARIES} ) # OK
# target_link_libraries( mediaspy ${QT_LIBRARIES} ${KDE4_KDEUI_LIBS} ${KDE4_KPARTS_LIBS} ) # NOK
