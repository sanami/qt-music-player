include (../common.pri)
TEMPLATE = app
QT += opengl network

CONFIG += precompile_header
# Use Precompiled headers (PCH)
PRECOMPILED_HEADER = pch.h

MOC_DIR = .moc
OBJECTS_DIR = .obj
MGEN_OUTDIR = .gen

target.path = $$M_INSTALL_BIN

INCLUDEPATH += ../../shared

SOURCES += main.cpp \
	app.cpp \
    ../../shared/web.cpp \
    ../../shared/settings.cpp \
    ../../shared/mplayer_control.cpp \
    ../../shared/media.cpp \
    ../../shared/logger.cpp \
    ../../shared/cookie_jar.cpp \
    ../../shared/qjson/serializerrunnable.cpp \
    ../../shared/qjson/serializer.cpp \
    ../../shared/qjson/qobjecthelper.cpp \
    ../../shared/qjson/parserrunnable.cpp \
    ../../shared/qjson/parser.cpp \
    ../../shared/qjson/json_scanner.cpp \
    ../../shared/qjson/json_parser.cc \
    main_page.cpp \
    stations_page.cpp

HEADERS += \
    app.h \
    pch.h \
    ../../shared/web.h \
    ../../shared/task.h \
    ../../shared/settings.h \
    ../../shared/playlist.h \
    ../../shared/mplayer_control.h \
    ../../shared/media.h \
    ../../shared/logger.h \
    ../../shared/cookie_jar.h \
    ../../shared/qjson/stack.hh \
    ../../shared/qjson/serializerrunnable.h \
    ../../shared/qjson/serializer.h \
    ../../shared/qjson/qobjecthelper.h \
    ../../shared/qjson/qjson_export.h \
    ../../shared/qjson/qjson_debug.h \
    ../../shared/qjson/position.hh \
    ../../shared/qjson/parserrunnable.h \
    ../../shared/qjson/parser_p.h \
    ../../shared/qjson/parser.h \
    ../../shared/qjson/location.hh \
    ../../shared/qjson/json_scanner.h \
    ../../shared/qjson/json_parser.hh \
    main_page.h \
    stations_page.h

MAKE_CLEAN += $$OBJECTS_DIR/*.o
MAKE_DISTCLEAN += \
    $$MOC_DIR/* $$MOC_DIR \
    $$OBJECTS_DIR/* $$OBJECTS_DIR \
    $$MGEN_OUTDIR/* $$MGEN_OUTDIR \

# Install
INSTALLS += target
