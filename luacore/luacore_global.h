#ifndef LUACORE_GLOBAL_H
#define LUACORE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(LUACORE_LIBRARY)
#  define LUACORESHARED_EXPORT Q_DECL_EXPORT
#else
#  define LUACORESHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // LUACORE_GLOBAL_H
