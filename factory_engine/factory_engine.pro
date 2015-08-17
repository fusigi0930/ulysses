TEMPLATE = app

QT += qml quick widgets

SOURCES += ../engine/factory/main.cpp

RESOURCES += ../engine/factory/qml.qrc

INCLUDEPATH += ../common/inc \
			   ../auto/inc

LIBS += \
	-L$$_PRO_FILE_PWD_/../auto/lib \
	-lnetio \
	-lxmlrun \
	-lcomio

CONFIG(debug, debug|release) {
	OUTDIR = $$_PRO_FILE_PWD_/../out/debug
}
CONFIG(release, debug|release) {
	OUTDIR = $$_PRO_FILE_PWD_/../out/release
}

OBJECTS_DIR = $$OUTDIR/factory/obj
MOC_DIR = $$OUTDIR/factory/obj/moc
DESTDIR = $$OUTDIR/bin

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)
