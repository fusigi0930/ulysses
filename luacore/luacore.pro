#-------------------------------------------------
#
# Project created by QtCreator 2016-05-23T11:03:19
#
#-------------------------------------------------

QT       += widgets network opengl sql script scripttools svg xml multimedia

TARGET = luacore
TEMPLATE = lib
CONFIG += dll warn_off

DEFINES += LUACORE_LIBRARY

SOURCES += luacore.cpp \
    luafunc.cpp \
    cluafunc.cpp \
    crunluathread.cpp

HEADERS += luacore.h \
        luacore_global.h \
    crunluathread.h \
    luna_four_code.h

INCLUDEPATH += \
        ../common/inc \
        $$_PRO_FILE_PWD_/lua-5.3.2/src

LIBS += \
    -L$$_PRO_FILE_PWD_/lua-5.3.2/src \
    -L$$_PRO_FILE_PWD_/opencv/lib \
    -llua

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
QMAKE_TARGET_DESCRIPTION = "ulysses PC version Lua engine"
QMAKE_TARGET_COPYRIGHT = "Copyright(c) 2016 Embux Inc. All rights reserved"
QMAKE_TARGET_COMPANY = "Embux Inc."

OBJECTS_DIR = $$OUTDIR/obj
MOC_DIR = $$OUTDIR/obj/moc
DESTDIR = $$OUTDIR/bin

lua_src.target = lua_src
lua_inst.target = lua_inst

win32 {
    QMAKE_POST_LINK += $$_PRO_FILE_PWD_/post_build.bat $$replace(_PRO_FILE_PWD_, /, \\) $$replace(OUTDIR, /, \\)\bin
    lua_src.commands = mingw32-make -C $$_PRO_FILE_PWD_/lua-5.3.2 mingw
    lua_inst.commands = mingw32-make -C $$_PRO_FILE_PWD_/lua-5.3.2 mingw
}

unix {
    target.path = /usr/lib
    lua_src.commands = make -f -C $$_PRO_FILE_PWD_/lua-5.3.2 linux
    INSTALLS += target
}

QMAKE_EXTRA_TARGETS += lua_src
PRE_TARGETDEPS = lua_src
