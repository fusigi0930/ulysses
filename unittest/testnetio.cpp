#include <QTest>
#include "netio.h"
#include <QList>
#include "debug.h"
#include "testgroup.h"
#include <QThread>
#include <QSignalSpy>

#if 0
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
	nio.close();
	nc.close();

	QCOMPARE(szRecv.left(4).compare("help"), 0);
}

void CTestGroup::testCNotifyRecv_broadcastProcess() {
	CNotifyRecv nrecv;
	nrecv.open();

	//void sigStartNewBootDev(int nPort);
	//void sigStartNewSysDev(QString szIP);
	//void sigHaltSysDev(QString szIP);
	QSignalSpy sigNewBootDev(&nrecv, SIGNAL(sigStartNewBootDev(int)));
	QSignalSpy sigNewSysDev(&nrecv, SIGNAL(sigStartNewSysDev(QString)));
	QSignalSpy sigHeltSysDev(&nrecv, SIGNAL(sigHaltSysDev(QString)));

	CNetcatIO nio;
	nio.open("net:client:127.0.0.1:7500");
	int i=0;
	while (10 > i++) {
		QString szData="ebxb:192.168.48.111";
		nio.write(szData);
		QThread::msleep(500);
	}

	QThread::msleep(4000);
	QCOMPARE(sigNewBootDev.count(), 1);

	QList<QVariant> arg=sigNewBootDev.takeFirst();
	QCOMPARE(arg.at(0).toInt(), 111);

	i=0;
	while (10 > i++) {
		QString szData="ebxs:factory:192.168.48.111";
		nio.write(szData);
		QThread::msleep(500);
	}
	QThread::msleep(4000);

	QCOMPARE(sigNewSysDev.count(), 1);
	arg.clear();
	arg=sigNewSysDev.takeFirst();
	QString szBuf=arg.at(0).toString();
	DMSG("arg: %s, %d", QSZ(szBuf), szBuf.length());
	QCOMPARE(szBuf.compare("factory:192.168.48.111"), 0);

	QCOMPARE(sigHeltSysDev.count(), 1);
	arg.clear();
	arg=sigHeltSysDev.takeFirst();
	szBuf=arg.at(0).toString();
	nrecv.close();
	nio.close();
	QThread::msleep(100);
	QCOMPARE(szBuf.compare("192.168.48.111"), 0);
}

void CTestGroup::testCTelnetIO_openClient() {
	CTelnetIO tio;
	tio.open("bbs.ntpu.edu.tw");
	QThread::msleep(12000);
	QString sz;
	tio.read(sz);
	DMSG("find ntpu: %d", sz.contains("ntpu"));
	int nPos=sz.indexOf("ntpu");
	QCOMPARE(sz.mid(nPos,4), QString("ntpu"));

	QString szCmd="guest\n";
	tio.write(szCmd);
	QThread::msleep(5000);
	szCmd="\n";
	tio.write(szCmd);
	QThread::msleep(5000);
	tio.read(sz);
	tio.close();
	nPos=sz.indexOf("Jobs");
	QCOMPARE(sz.mid(nPos,4), QString("Jobs"));
}

#endif
