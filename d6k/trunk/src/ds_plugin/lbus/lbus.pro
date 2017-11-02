#-------------------------------------------------
#
# Project created by QtCreator 2016-05-11T16:32:06
#
#-------------------------------------------------


TARGET = lbus
TEMPLATE = lib

DESTDIR = ../../../bin
QT += core network xml widgets gui 

DEFINES += DS_LIBRARY WIN64

INCLUDEPATH += ./GeneratedFiles \
    . \
    ./GeneratedFiles/Debug \
     $(QTDIR)/mkspecs/win32-msvc \
    ./../../../include/  
    
DEPENDPATH += .
MOC_DIR += ./GeneratedFiles/debug
OBJECTS_DIR += debug
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles
win32:RC_FILE = lbus.rc

SOURCES +=  lbus.cpp \
            lbus_module.cpp 
          

HEADERS += lbus.h \
           lbus_module.h \      
         ../../../include/devicestudio/plugin_module.h \

        
TRANSLATIONS += ./lbus-zh-CN.ts
RESOURCES += lbus.qrc
      
#FORMS += ./ui/coildlg.ui    
unix {
    target.path = /usr/lib
    INSTALLS += target
}
