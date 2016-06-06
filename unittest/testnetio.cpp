#include <QTest>
#include "netio.h"
#include <QList>
#include "debug.h"
#include "testgroup.h"
#include <QThread>

void CTestGroup::testCNetcatIO_openServer() {
	QString szRecv;

	CNetcatIO nio, nc;
	nio.open("net:server:6666");
	nc.open("net:client:127.0.0.1:6666");
	char abuf[]="help\r";
	nc.write(abuf, strlen(abuf));
	QThread::msleep(1000);
	nio.read(szRecv);
	DMSG("recv: %s", QSZ(szRecv));

	QCOMPARE(szRecv.left(4).compare("help"), 0);
}

