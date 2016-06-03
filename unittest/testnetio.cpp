#include <QTest>
#include "netio.h"
#include <QList>
#include "debug.h"
#include "testgroup.h"

void CTestGroup::testCNetcatIO_openServer() {
	QString szRecv;

	CNetcatIO nio, nc;
	nio.open("net:server:6666");
	nc.open("net:client:192.168.0.151:6666");
	char abuf[]="help\r";
	nc.write(abuf, strlen(abuf));
	nio.read(szRecv);


	QCOMPARE(szRecv.left(4).compare("help"), 0);
}

