#-------------------------------------------------
#
# Project created by QtCreator 2018-06-04T14:41:23
#
#-------------------------------------------------

QT       -= core gui

TARGET = notepad_plugins
TEMPLATE = lib

DEFINES += NOTEPAD_PLUGINS_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/DockingFeature/GoToLineDlg.cpp \
    src/DockingFeature/StaticDialog.cpp \
    src/NppPluginDemo.cpp \
    src/PluginDefinition.cpp \
    src/unix_convertor.cpp \
    src/ip_convertor.cpp

HEADERS += \
    src/DockingFeature/Docking.h \
    src/DockingFeature/DockingDlgInterface.h \
    src/DockingFeature/dockingResource.h \
    src/DockingFeature/GoToLineDlg.h \
    src/DockingFeature/resource.h \
    src/DockingFeature/StaticDialog.h \
    src/DockingFeature/Window.h \
    src/menuCmdID.h \
    src/Notepad_plus_msgs.h \
    src/PluginDefinition.h \
    src/PluginInterface.h \
    src/Scintilla.h \
    src/unix_convertor.h \
    src/ip_convertor.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

unix|win32: LIBS += -lshlwapi
