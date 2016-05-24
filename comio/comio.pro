#-------------------------------------------------
#
# Project created by QtCreator 2015-08-17T23:00:00
#
#-------------------------------------------------

QT       -= gui
QT	+=	serialport

TARGET = comio
TEMPLATE = lib
CONFIG += dll

DEFINES += COMIO_LIBRARY

SOURCES += comio.cpp

HEADERS += comio.h\
        comio_global.h

INCLUDEPATH += ../common/inc

CONFIG(debug, debug|release) {
	OUTDIR = $$_PRO_FILE_PWD_/../out/debug
}
CONFIG(release, debug|release) {
	OUTDIR = $$_PRO_FILE_PWD_/../out/release
}

OBJECTS_DIR = $$OUTDIR/obj
MOC_DIR = $$OUTDIR/obj/moc
DESTDIR = $$OUTDIR/bin

win32 {
	QMAKE_POST_LINK += $$_PRO_FILE_PWD_/post_build.bat $$replace(_PRO_FILE_PWD_, /, \\) $$replace(OUTDIR, /, \\)\bin
}


unix {
    target.path = /usr/lib
    INSTALLS += target
}
