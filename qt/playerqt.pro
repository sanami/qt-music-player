#-------------------------------------------------
#
# Project created by QtCreator 2011-03-29T19:41:18
#
#-------------------------------------------------

TEMPLATE = app
TARGET = playerqt

QT       += core gui network phonon maemo5
CONFIG += precompile_header mobility
MOBILITY = 

MOC_DIR = .moc
OBJECTS_DIR = .obj
UI_DIR = .ui

INCLUDEPATH += ../shared

# Use Precompiled headers (PCH)
PRECOMPILED_HEADER = pch.h

SOURCES += main.cpp\
        app.cpp \
    ../shared/web.cpp \
    ../shared/qjson/serializerrunnable.cpp \
    ../shared/qjson/serializer.cpp \
    ../shared/qjson/qobjecthelper.cpp \
    ../shared/qjson/parserrunnable.cpp \
    ../shared/qjson/parser.cpp \
    ../shared/qjson/json_scanner.cpp \
    ../shared/qjson/json_parser.cc \
    info_page.cpp \
    ../shared/media.cpp \
    ../shared/mplayer_control.cpp \
    ../shared/settings.cpp \
    player_page.cpp \
    ../shared/cookie_jar.cpp \
    log_page.cpp \
    ../shared/logger.cpp \
    stations_page.cpp \
    filter_page.cpp \
    playlist_page.cpp \
    ../shared/playlist_manager.cpp \
    app_window.cpp

HEADERS  += app.h \
    info_page.h \
    player_page.h \
    log_page.h \
    stations_page.h \
    filter_page.h \
    ../shared/web.h \
    ../shared/task.h \
    ../shared/qjson/stack.hh \
    ../shared/qjson/serializerrunnable.h \
    ../shared/qjson/serializer.h \
    ../shared/qjson/qobjecthelper.h \
    ../shared/qjson/qjson_export.h \
    ../shared/qjson/qjson_debug.h \
    ../shared/qjson/position.hh \
    ../shared/qjson/parserrunnable.h \
    ../shared/qjson/parser.h \
    ../shared/qjson/parser_p.h \
    ../shared/qjson/location.hh \
    ../shared/qjson/json_scanner.h \
    ../shared/qjson/json_parser.hh \
    ../shared/media.h \
    ../shared/mplayer_control.h \
    ../shared/settings.h \
    ../shared/cookie_jar.h \
    ../shared/logger.h \
    playlist_page.h \
    pch.h \
    ../shared/data.h \
    ../shared/playlist_manager.h \
    ../shared/playlist.h \
    app_window.h

FORMS    += \
    info_page.ui \
    player_page.ui \
    log_page.ui \
    stations_page.ui \
    filter_page.ui \
    playlist_page.ui \
    app_window.ui

symbian {
    TARGET.UID3 = 0xe7928591
    # TARGET.CAPABILITY += 
    TARGET.EPOCSTACKSIZE = 0x14000
    TARGET.EPOCHEAPSIZE = 0x020000 0x800000
}

OTHER_FILES += \
    qtc_packaging/debian_fremantle/rules \
    qtc_packaging/debian_fremantle/README \
    qtc_packaging/debian_fremantle/copyright \
    qtc_packaging/debian_fremantle/control \
    qtc_packaging/debian_fremantle/compat \
    qtc_packaging/debian_fremantle/changelog \
    ../doc/TODO

maemo5 {
	desktop.files = qtc_packaging/playerqt.desktop
    desktop.path = /usr/share/applications/hildon

    target.path = /opt/playerqt/bin
	INSTALLS += target desktop
}

RESOURCES += \
    resources.qrc
