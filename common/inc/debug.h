#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <qDebug>
#include <QDateTime>

#define QSZ(sz) sz.toUtf8().data()

#ifdef DEBUG
#define DMSG(e,...) \
    qDebug("%lld [%s:%d] " e "\n", QDateTime::currentMSecsSinceEpoch(), __PRETTY_FUNCTION__, __LINE__, ##__VA_ARGS__)

#else
#define DMSG(e,...) \
	do {} while(0)
#endif

#endif // __DEBUG_H__

