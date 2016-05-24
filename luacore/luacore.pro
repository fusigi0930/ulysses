#-------------------------------------------------
#
# Project created by QtCreator 2016-05-23T11:03:19
#
#-------------------------------------------------

QT       += widgets network opengl sql script scripttools svg xml multimedia

TARGET = luacore
TEMPLATE = lib
CONFIG += dll

DEFINES += LUACORE_LIBRARY

SOURCES += cluacore.cpp

HEADERS += cluacore.h\
        luacore_global.h

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

lua_src.target = lua_src

win32 {
    QMAKE_POST_LINK += $$_PRO_FILE_PWD_/post_build.bat $$replace(_PRO_FILE_PWD_, /, \\) $$replace(OUTDIR, /, \\)\bin
    lua_src.commands = mingw32-make -C $$_PRO_FILE_PWD_/lua-5.3.2 mingw
}

unix {
    target.path = /usr/lib
    lua_src.commands = make -f -C $$_PRO_FILE_PWD_/lua-5.3.2 linux
    INSTALLS += target
}

QMAKE_EXTRA_TARGETS += lua_src
PRE_TARGETDEPS = lua_src
