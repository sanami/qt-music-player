include (common.pri)
TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS = src translations

# Theme files
theme.files = ./theme/*
theme.path = $$M_THEME_DIR/$$TARGET/
theme.CONFIG += no_check_exist

desktop.files = ./data/*.desktop
desktop.path = /usr/share/applications

INSTALLS += theme desktop
