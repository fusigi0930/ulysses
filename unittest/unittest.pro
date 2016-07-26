#-------------------------------------------------
#
# Project created by QtCreator 2015-08-16T14:48:24
#
#-------------------------------------------------

QT       += core testlib network sql
QT       -= gui

TARGET = unittest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    testxmlrun.cpp \
    testluacore.cpp \
    testnetio.cpp \
    testresultcore.cpp

INCLUDEPATH += ../common/inc \
			   ../auto/inc

LIBS += \
	-L$$_PRO_FILE_PWD_/../auto/lib \
	-lnetio \
	-lxmlrun \
    -lxmlcfg \
    -lluacore \
    -lresultcore

CONFIG(debug, debug|release) {
	OUTDIR = $$_PRO_FILE_PWD_/../out/debug
}
CONFIG(release, debug|release) {
	OUTDIR = $$_PRO_FILE_PWD_/../out/release
}

GITVER = $$system("genver.sh")

VERSION = $$GITVER
QMAKE_TARGET_PRODUCT = "ulysses - unit test"
QMAKE_TARGET_DESCRIPTION = "ulysses unit test tool"
QMAKE_TARGET_COPYRIGHT = "Copyright(c) 2016 Embux Inc. All rights reserved"
QMAKE_TARGET_COMPANY = "Embux Inc."

OBJECTS_DIR = $$OUTDIR/test/obj
MOC_DIR = $$OUTDIR/obj/test/moc
DESTDIR = $$OUTDIR/bin

HEADERS += \
    testgroup.h
