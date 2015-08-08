#ifndef NETIO_GLOBAL_H
#define NETIO_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(NETIO_LIBRARY)
#  define NETIOSHARED_EXPORT Q_DECL_EXPORT
#else
#  define NETIOSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // NETIO_GLOBAL_H
