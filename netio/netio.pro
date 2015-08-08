#-------------------------------------------------
#
# Project created by QtCreator 2015-08-08T17:23:54
#
#-------------------------------------------------

QT       += network

QT       -= gui

TARGET = netio
TEMPLATE = lib

DEFINES += NETIO_LIBRARY

SOURCES += netio.cpp

HEADERS += netio.h\
        netio_global.h

INCLUDEPATH += ../common/inc

debug: OUTDIR = $$_PRO_FILE_PWD_/../out/debug
release: OUTDIR = $$_PRO_FILE_PWD_/../out/release

OBJECTS_DIR = $$OUTDIR/obj
MOC_DIR = $$OUTDIR/obj/moc
DESTDIR = $$OUTDIR/bin

unix {
    target.path = /usr/lib
    INSTALLS += target
}
