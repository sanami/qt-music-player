#-------------------------------------------------
#
# Project created by QtCreator 2011-04-09T16:43:47
#
#-------------------------------------------------

QT       += testlib core gui network

TARGET = tests
CONFIG   += precompile_header console 
CONFIG   -= app_bundle

TEMPLATE = app

# Use Precompiled headers (PCH)
PRECOMPILED_HEADER = ../../pch.h

SOURCES += \
    test.cpp \
	../../mplayer_control.cpp \
    ../../settings.cpp \
	../../cookie_jar.cpp \
    ../../web.cpp \
    ../../qjson/serializerrunnable.cpp \
    ../../qjson/serializer.cpp \
    ../../qjson/qobjecthelper.cpp \
    ../../qjson/parserrunnable.cpp \
    ../../qjson/parser.cpp \
    ../../qjson/json_scanner.cpp \
    ../../qjson/json_parser.cc

HEADERS += \
    test.h \
	../../mplayer_control.h \
    ../../settings.h \
	../../cookie_jar.h \
    ../../pch.h \
    ../../web.h \
    ../../qjson/stack.hh \
    ../../qjson/serializerrunnable.h \
    ../../qjson/serializer.h \
    ../../qjson/qobjecthelper.h \
    ../../qjson/qjson_export.h \
    ../../qjson/qjson_debug.h \
    ../../qjson/position.hh \
    ../../qjson/parserrunnable.h \
    ../../qjson/parser_p.h \
    ../../qjson/parser.h \
    ../../qjson/location.hh \
    ../../qjson/json_scanner.h \
    ../../qjson/json_parser.hh

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/bin
    } else {
        target.path = /usr/local/bin
    }
    INSTALLS += target
}
