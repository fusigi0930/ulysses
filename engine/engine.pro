TEMPLATE = app

QT += qml quick widgets

SOURCES += main.cpp

RESOURCES += qml.qrc

INCLUDEPATH += ../common/inc

debug: OUTDIR = $$_PRO_FILE_PWD_/../out/debug
release: OUTDIR = $$_PRO_FILE_PWD_/../out/release

OBJECTS_DIR = $$OUTDIR/obj
MOC_DIR = $$OUTDIR/obj/moc
DESTDIR = $$OUTDIR/bin

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)