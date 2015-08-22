#ifndef XMLCFG_GLOBAL_H
#define XMLCFG_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(XMLCFG_LIBRARY)
#  define XMLCFGSHARED_EXPORT Q_DECL_EXPORT
#else
#  define XMLCFGSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // XMLCFG_GLOBAL_H
