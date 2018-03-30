#-------------------------------------------------
#
# Project created by QtCreator 2016-05-11T16:32:06
#
#-------------------------------------------------


TARGET = osc
TEMPLATE = lib

DESTDIR = ../../../bin
QT += core network xml  gui 

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

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
win32:RC_FILE = osc.rc

SOURCES +=  osc.cpp \
            osc_module.cpp \
            wave_info.cpp \
            childfrm.cpp \
            left_view.cpp \
            right_view.cpp \
            borderlayout.cpp \
            channelview_info.cpp\
            colour_table.cpp \
            screenshot.cpp \
            amplifier.cpp \
            common_helper.cpp     \      

HEADERS += osc.h \
           osc_module.h \      
           ../../../include/devicestudio/plugin_module.h \
	       osc_treeitem_def.h \
           wave_info.h \
           childfrm.h \
           left_view.h \
           right_view.h \
           borderlayout.h \ 
           stl_util-inl.h \
           colour_define.h \
           channelview_info.h \
           colour_table.h \
           screenshot.h \
           amplifier.h \
           common_helper.h     \               
        
TRANSLATIONS += ./osc-zh-CN.ts
RESOURCES += osc.qrc
      
#FORMS += ./ui/coildlg.ui    
unix {
    target.path = /usr/lib
    INSTALLS += target
}
