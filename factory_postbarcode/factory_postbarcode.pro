TEMPLATE = app

QT += qml quick widgets sql

SOURCES += main.cpp \
    cinterfaceui.cpp

RESOURCES += qml.qrc

INCLUDEPATH += ../common/inc \
			   ../auto/inc

LIBS += \
	-L$$_PRO_FILE_PWD_/../auto/lib \
	-lnetio \
	-lxmlrun \
	-lcomio \
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
QMAKE_TARGET_PRODUCT = "ulysses - fleet"
QMAKE_TARGET_DESCRIPTION = "ulysses version factory post scan tool"
QMAKE_TARGET_COPYRIGHT = "Copyright(c) 2016 Embux Inc. All rights reserved"
QMAKE_TARGET_COMPANY = "Embux Inc."

OBJECTS_DIR = $$OUTDIR/factory_post/obj
MOC_DIR = $$OUTDIR/factory_post/obj/moc
DESTDIR = $$OUTDIR/bin

ulysses_common.target = ulysses_common
ulysses_common.commands = rcc -binary $$_PRO_FILE_PWD_/../common/qml/ulysses_common.qrc -o $$DESTDIR/ulysses_common.rcc
QMAKE_EXTRA_TARGETS += ulysses_common
PRE_TARGETDEPS = ulysses_common

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    cinterfaceui.h
