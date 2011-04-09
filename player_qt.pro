#-------------------------------------------------
#
# Project created by QtCreator 2011-03-29T19:41:18
#
#-------------------------------------------------

TEMPLATE = app
TARGET = playerqt

QT       += core gui network phonon
CONFIG += precompile_header mobility
MOBILITY = 

#LIBS += -L../qjson-build-simulator/lib -lqjson

# Use Precompiled headers (PCH)
PRECOMPILED_HEADER = pch.h

SOURCES += main.cpp\
        form.cpp \
    web.cpp \
    qjson/serializerrunnable.cpp \
    qjson/serializer.cpp \
    qjson/qobjecthelper.cpp \
    qjson/parserrunnable.cpp \
    qjson/parser.cpp \
    qjson/json_scanner.cpp \
    qjson/json_parser.cc \
    info_page.cpp \
    media.cpp \
	mplayer_control.cpp \
    settings.cpp

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
    qjson/json_parser.hh \
    info_page.h \
    media.h \
	mplayer_control.h \
    settings.h

FORMS    += form.ui \
    info_page.ui

symbian {
    TARGET.UID3 = 0xe7928591
    # TARGET.CAPABILITY += 
    TARGET.EPOCSTACKSIZE = 0x14000
    TARGET.EPOCHEAPSIZE = 0x020000 0x800000
}
