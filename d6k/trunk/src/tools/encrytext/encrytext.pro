# ----------------------------------------------------
# This file is generated by the Qt Visual Studio Tools.
# ------------------------------------------------------

TEMPLATE = app
TARGET = encrytext
DESTDIR = ./bin
QT += core widgets gui
CONFIG += debug
DEFINES += _WINDOWS QT_DEPRECATED_WARNINGS QT_WIDGETS_LIB
INCLUDEPATH += . \
    ./debug \
    . \
    $(QTDIR)/mkspecs/win32-msvc2015 \
    ./GeneratedFiles
LIBS += -lshell32 \
    -l./lib/TZhuZhanDll
DEPENDPATH += .
MOC_DIR += ./GeneratedFiles/debug
OBJECTS_DIR += debug
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles
HEADERS += ./include/TZhuZhanDll.h \
    ./encrycontrolwgt.h
SOURCES += ./encrycontrolwgt.cpp \
    ./main.cpp
FORMS += ./encrycontrolwgt.ui