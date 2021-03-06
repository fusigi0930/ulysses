#-------------------------------------------------
#
# Project created by QtCreator 2016-06-29T13:57:58
#
#-------------------------------------------------

QT       += sql network

QT       -= gui

TARGET = resultcore
TEMPLATE = lib
CONFIG += dll

QTPLUGIN += QSQLMYSQL

DEFINES += RESULTCORE_LIBRARY

SOURCES += csqlitestore.cpp \
    culyssesstore.cpp \
    cdbsyncstore.cpp

HEADERS += csqlitestore.h\
        resultcore_global.h

INCLUDEPATH += ../common/inc

CONFIG(debug, debug|release) {
    OUTDIR = $$_PRO_FILE_PWD_/../out/debug
}
CONFIG(release, debug|release) {
    OUTDIR = $$_PRO_FILE_PWD_/../out/release
}

GITVER = $$system("genver.sh")

VERSION = $$GITVER
CONFIG += skip_target_version_ext
QMAKE_TARGET_PRODUCT = "ulysses - libraries"
QMAKE_TARGET_DESCRIPTION = "ulysses data store library"
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
