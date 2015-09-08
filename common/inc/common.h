#ifndef __COMMON_H__
#define __COMMON_H__
#include <QTime>

#define busy_msleep(m) { \
	QTime endTime=QTime::currentTime().addMSec(m); \
	while (endTime > QTime::currentTime()) { \
		QCoreApplication::processEvents(QEventLoop::AllEvents, 1); \
	} \
}

#endif