#-------------------------------------------------
#
# Project created by QtCreator 2018-01-21T09:09:15
#
#-------------------------------------------------
#INCLUDEPATH += /usr/include/KF5

unix {
        CONFIG += link_pkgconfig
        PKGCONFIG += poppler-qt5
}

QT       += widgets sql printsupport multimediawidgets
QT       += KXmlGui KIconThemes  KParts

TARGET = mygui
TEMPLATE = lib

DEFINES += MYGUI_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += mygui.cpp \
    guimainwindow.cpp \
    guirepository.cpp \
    guirepositoryimpl.cpp \
    guicontroler.cpp \
    guimode.cpp \
    guiservice.cpp \
    pobjecticonview.cpp \
    propertyeditor.cpp \
    splashscreen.cpp \
    splashscreenimpl.cpp \
    editorbase.cpp \
    dateeditor.cpp \
    formspopupfactory.cpp \
    genericmapviewcontroler.cpp \
    guiconfig.cpp \
    guiobject.cpp \
    guiobjectfactoryimpl.cpp \
    guiobjectmapper.cpp \
    guipopupfactory.cpp \
    mapviewspopupfactory.cpp \
    pobjectdialog.cpp \
    pobjecteditor.cpp \
    pobjecteditor2.cpp \
    pobjecteditor3.cpp \
    pobjectlabel.cpp \
    pobjecttable.cpp \
    pobjecttablecontrolerimpl.cpp \
    stringeditor.cpp \
    textpropertyeditor.cpp \
    pdfview/actionhandler.cpp \
    pdfview/bookmarkshandler.cpp \
    pdfview/filesettings.cpp \
    pdfview/globallocale.cpp \
    pdfview/pageitem.cpp \
    pdfview/pdfview.cpp \
    pdfview/printhandler.cpp \
    pdfview/selectaction.cpp \
    pdfview/selectpageaction.cpp \
    pdfview/synctexhandler.cpp \
    pdfview/zoomaction.cpp \
    pdfviewer.cpp \
    pobjectdata.cpp \
    pobjectdroptarget.cpp \
    pobjectgraphicsitem.cpp \
    pobjectgraphicsitemmapper.cpp \
    pobjectgraphicsitemnp.cpp \
    pobjecticonviewitem.cpp \
    pobjectlistprovider.cpp \
    textpropertybrowser.cpp \
    textpropertyviewer.cpp \
    pdfview/synctex/synctex_parser.c \
    pdfview/synctex/synctex_parser_utils.c \
    abstractpropertyeditor.cpp \
    editrequestor.cpp \
    genericmapmapper.cpp \
    myguiobjectfactory.cpp \
    pobjectlistview.cpp \
    pobjectlistviewitem.cpp \
    filter/abstractfilter.cpp \
    filter/filter.cpp \
    filter/namefilter.cpp \
    filter/pobjectpropertyfilter.cpp \
    filter/propertyfilter.cpp \
    filter/stringpropertyfilter.cpp \
    filter/typefilter.cpp \
    genericmap.cpp \
    genericmapview.cpp \
    guiobjectfactory.cpp \
    pobjectmultiview.cpp \
    mapperview.cpp \
    xmlexportdialog.cpp \
    xmlimportdialog.cpp \
    reports/latexoutput.cpp \
    booleaneditor.cpp \
    configurepobjecticonviewaction.cpp \
    datenpopup.cpp \
    repositoryitem.cpp \
    urleditor.cpp \
    datecompare.cpp \
    fileeditor.cpp \
    filtereditor.cpp \
    namedobjectptrcomp.cpp \
    pobjectcombobox.cpp \
    propertychooser.cpp \
    propertyitem.cpp \
    relateaction.cpp \
    rootitem.cpp \
    xmlfactory.cpp \
    standardguimode.cpp \
    formarea.cpp \
    movieplayer.cpp

HEADERS += mygui.h\
        mygui_global.h \
    guimainwindow.h \
    guirepository.h \
    guirepositoryimpl.h \
    guicontroler.h \
    guimode.h \
    guiservice.h \
    pobjecticonview.h \
    propertyeditor.h \
    splashscreen.h \
    splashscreenimpl.h \
    editorbase.h \
    dateeditor.h \
    formspopupfactory.h \
    genericmapviewcontroler.h \
    guiconfig.h \
    guiobject.h \
    guiobjectfactoryimpl.h \
    guiobjectmapper.h \
    guipopupfactory.h \
    mapviewspopupfactory.h \
    pobjectdialog.h \
    pobjecteditor.h \
    pobjecteditor2.h \
    pobjecteditor3.h \
    pobjectlabel.h \
    pobjecttable.h \
    pobjecttablecontrolerimpl.h \
    stringeditor.h \
    textpropertyeditor.h \
    pdfview/synctex/synctex_parser.h \
    pdfview/synctex/synctex_parser_local.h \
    pdfview/synctex/synctex_parser_utils.h \
    pdfview/actionhandler.h \
    pdfview/bookmarkshandler.h \
    pdfview/filesettings.h \
    pdfview/globallocale.h \
    pdfview/pageitem.h \
    pdfview/pdfview.h \
    pdfview/pdfview_p.h \
    pdfview/printhandler.h \
    pdfview/selectaction.h \
    pdfview/selectpageaction.h \
    pdfview/synctexhandler.h \
    pdfview/zoomaction.h \
    pdfviewer.h \
    pobjectdata.h \
    pobjectdroptarget.h \
    pobjectgraphicsitem.h \
    pobjectgraphicsitemmapper.h \
    pobjectgraphicsitemnp.h \
    pobjecticonviewitem.h \
    pobjectlistprovider.h \
    textpropertybrowser.h \
    textpropertyviewer.h \
    abstractpropertyeditor.h \
    editrequestor.h \
    genericmapmapper.h \
    myguiobjectfactory.h \
    pobjectlistview.h \
    pobjectlistviewitem.h \
    filter/abstractfilter.h \
    filter/filter.h \
    filter/namefilter.h \
    filter/pobjectpropertyfilter.h \
    filter/propertyfilter.h \
    filter/stringpropertyfilter.h \
    filter/typefilter.h \
    genericmap.h \
    genericmapview.h \
    guiobjectfactory.h \
    pobjectmultiview.h \
    mapperview.h \
    xmlexportdialog.h \
    xmlimportdialog.h \
    reports/latexoutput.h \
    booleaneditor.h \
    configurepobjecticonviewaction.h \
    datenpopup.h \
    repositoryitem.h \
    urleditor.h \
    datecompare.h \
    fileeditor.h \
    filtereditor.h \
    namedobjectptrcomp.h \
    pobjectcombobox.h \
    propertychooser.h \
    propertyitem.h \
    relateaction.h \
    rootitem.h \
    xmlfactory.h \
    standardguimode.h \
    formarea.h \
    movieplayer.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

unix:!macx: LIBS += -L$$PWD/../myorm/ -lmyorm

INCLUDEPATH += $$PWD/../myorm
DEPENDPATH += $$PWD/../myorm

DISTFILES += \
    persistence.xml

FORMS += \
    urleditor.ui \
    formarea.ui
