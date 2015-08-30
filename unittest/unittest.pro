#-------------------------------------------------
#
# Project created by QtCreator 2015-08-16T14:48:24
#
#-------------------------------------------------

QT       += core testlib
QT       -= gui

TARGET = unittest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    testxmlrun.cpp

INCLUDEPATH += ../common/inc \
			   ../auto/inc

LIBS += \
	-L$$_PRO_FILE_PWD_/../auto/lib \
	-lnetio \
	-lxmlrun \
	-lxmlcfg

CONFIG(debug, debug|release) {
	OUTDIR = $$_PRO_FILE_PWD_/../out/debug
}
CONFIG(release, debug|release) {
	OUTDIR = $$_PRO_FILE_PWD_/../out/release
}

OBJECTS_DIR = $$OUTDIR/test/obj
MOC_DIR = $$OUTDIR/obj/test/moc
DESTDIR = $$OUTDIR/bin
