#-------------------------------------------------
#
# Project created by QtCreator 2017-10-04T15:50:52
#
#-------------------------------------------------

QT       += network xml gui
 

TARGET = ft_modbustcp
TEMPLATE = lib


DESTDIR = ../../../bin

DEFINES += FT_DRIVER WIN64


INCLUDEPATH += ./GeneratedFiles \
    . \
    ./GeneratedFiles/Debug \
    ./../../../include/  

    
DEPENDPATH += .
MOC_DIR += ./GeneratedFiles/debug
OBJECTS_DIR += debug
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles
win32:RC_FILE = ft_modbustcp.rc

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
        modbus_ftdrvier.cpp

HEADERS += \
        modbus_ftdrvier.h \
        ../../../include/log/log.h \     
        ../../../include/fesapi/ftdriver_main.h \
        ../../../include/fesapi/fesdatadef.h \
        ../../../include/fesapi/fesapi.h \


LIBS += \
        -L../../../lib/\
	-llog\
	-lfesapi \        

unix {
    target.path = /usr/lib
    INSTALLS += target
}
