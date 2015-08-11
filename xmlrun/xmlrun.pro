#-------------------------------------------------
#
# Project created by QtCreator 2015-08-11T23:28:49
#
#-------------------------------------------------

QT       += xml

QT       -= gui

TARGET = xmlrun
TEMPLATE = lib

DEFINES += XMLRUN_LIBRARY

SOURCES += xmlrun.cpp

HEADERS += xmlrun.h\
        xmlrun_global.h

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
