#-------------------------------------------------
#
# Project created by QtCreator 2015-08-08T17:23:54
#
#-------------------------------------------------

QT       += network

QT       -= gui

TARGET = netio
TEMPLATE = lib
CONFIG += dll

DEFINES += NETIO_LIBRARY

SOURCES += netio.cpp \
    iothread.cpp \
    notifyrecv.cpp \
    telnetio.cpp

HEADERS += netio.h\
        netio_global.h

# QMAKE_CXXFLAGS += -fpermissive

INCLUDEPATH += ../common/inc

win32 {
LIBS += \
    -lws2_32
}

CONFIG(debug, debug|release) {
	OUTDIR = $$_PRO_FILE_PWD_/../out/debug
}
CONFIG(release, debug|release) {
	OUTDIR = $$_PRO_FILE_PWD_/../out/release
}

GITVER = $$system("genver.sh")

VERSION = $$GITVER
QMAKE_TARGET_PRODUCT = "ulysses - libraries"
QMAKE_TARGET_DESCRIPTION = "ulysses network library"
QMAKE_TARGET_COPYRIGHT = "Copyright(c) 2016 Embux Inc. All rights reserved"
QMAKE_TARGET_COMPANY = "Embux Inc."

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
