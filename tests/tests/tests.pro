#-------------------------------------------------
#
# Project created by QtCreator 2011-04-09T16:43:47
#
#-------------------------------------------------

QT       += core testlib

QT       -= gui

TARGET = tests
CONFIG   += precompile_header console 
CONFIG   -= app_bundle

TEMPLATE = app

# Use Precompiled headers (PCH)
PRECOMPILED_HEADER = ../../pch.h

SOURCES += \
    test.cpp \
    ../../player_control.cpp

HEADERS += \
    test.h \
    ../../player_control.h
