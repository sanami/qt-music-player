#-------------------------------------------------
#
# Project created by QtCreator 2011-03-29T19:41:18
#
#-------------------------------------------------

QT       += core gui network

TARGET = playerqt
TEMPLATE = app


SOURCES += main.cpp\
        form.cpp \
    web.cpp \
    qjson/serializerrunnable.cpp \
    qjson/serializer.cpp \
    qjson/qobjecthelper.cpp \
    qjson/parserrunnable.cpp \
    qjson/parser.cpp \
    qjson/json_scanner.cpp \
    qjson/json_parser.cc

HEADERS  += form.h \
    web.h \
    task.h \
    qjson/stack.hh \
    qjson/serializerrunnable.h \
    qjson/serializer.h \
    qjson/qobjecthelper.h \
    qjson/qjson_export.h \
    qjson/qjson_debug.h \
    qjson/position.hh \
    qjson/parserrunnable.h \
    qjson/parser.h \
    qjson/parser_p.h \
    qjson/location.hh \
    qjson/json_scanner.h \
    qjson/json_parser.hh

FORMS    += form.ui

CONFIG += mobility
MOBILITY = 

#LIBS += -L../qjson-build-simulator/lib -lqjson

symbian {
    TARGET.UID3 = 0xe7928591
    # TARGET.CAPABILITY += 
    TARGET.EPOCSTACKSIZE = 0x14000
    TARGET.EPOCHEAPSIZE = 0x020000 0x800000
}
