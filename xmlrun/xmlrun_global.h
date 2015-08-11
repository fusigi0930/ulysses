#ifndef XMLRUN_GLOBAL_H
#define XMLRUN_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(XMLRUN_LIBRARY)
#  define XMLRUNSHARED_EXPORT Q_DECL_EXPORT
#else
#  define XMLRUNSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // XMLRUN_GLOBAL_H
