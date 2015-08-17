#ifndef COMIO_GLOBAL_H
#define COMIO_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(COMIO_LIBRARY)
#  define COMIOSHARED_EXPORT Q_DECL_EXPORT
#else
#  define COMIOSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // COMIO_GLOBAL_H
