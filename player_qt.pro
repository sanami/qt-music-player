#-------------------------------------------------
#
# Project created by QtCreator 2011-03-29T19:41:18
#
#-------------------------------------------------

QT       += core gui network

TARGET = player_qt
TEMPLATE = app


SOURCES += main.cpp\
        form.cpp

HEADERS  += form.h

FORMS    += form.ui

CONFIG += mobility
MOBILITY = 

LIBS += -L../qjson-build-simulator/lib -lqjson

symbian {
    TARGET.UID3 = 0xe7928591
    # TARGET.CAPABILITY += 
    TARGET.EPOCSTACKSIZE = 0x14000
    TARGET.EPOCHEAPSIZE = 0x020000 0x800000
}
