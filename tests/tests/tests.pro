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
    ../../cookiejar.cpp \
    ../../web.cpp

HEADERS += \
    test.h \
	../../mplayer_control.h \
    ../../settings.h \
    ../../cookiejar.h \
    ../../pch.h \
    ../../web.h
