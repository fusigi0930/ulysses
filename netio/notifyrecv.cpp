#include "netio.h"
#include "debug.h"

#include <QRegExp>

#include <QDateTime>
#include <QTime>

#ifdef Q_OS_WIN
#include <winsock2.h>
#endif

#include <unistd.h>
#include <algorithm>

#define UBOOT_BCAST_PREFIX "ebxb"
#define SYS_BCAST_PREFIX "ebxs"

///////////////////////////////////////////////////////
/// \brief CNotifyRecv
///

CNotifyRecv::CNotifyRecv() : CNetcatIO() {
	m_parserThread=NULL;
	m_hParserThread=NULL;
}

CNotifyRecv::~CNotifyRecv() {

}

bool CNotifyRecv::open() {
	bool bRet=CNetcatIO::open("net:server:7500");
	m_parserThread = new CIOParserThread;
	m_parserThread->setIO(this);
	m_parserThread->start();
	return bRet;
}

void CNotifyRecv::close() {
	if (NULL != m_parserThread) {
		if (m_parserThread->isRunning())
			m_parserThread->terminate();
		delete m_parserThread;
		m_parserThread=NULL;
	}
	m_hParserThread=NULL;

	CNetcatIO::close();
}

int CNotifyRecv::runParser() {
#ifdef Q_OS_WIN
	m_hParserThread=::GetCurrentThread();
#endif
	while (-1 != m_io) {
		QThread::msleep(3000);
		parseBroadcast();
	}
#ifdef Q_OS_WIN
	m_hParserThread=NULL;
#endif
	return 0;
}

int CNotifyRecv::parseBroadcast() {
	//m_szRecvData.replace('\r', '\n');
	QStringList dataList=m_szRecvData.split('\n', QString::SkipEmptyParts);
	QStringList::iterator data;

	// for bootloader
	for (std::map<int, QString>::iterator pBootDev=m_mapBootDev.begin();
		pBootDev != m_mapBootDev.end(); pBootDev++) {
		QString szData=QString().sprintf("ebxb:%s", QSZ(pBootDev->second));
		if (-1 == dataList.indexOf(szData)) {
			emit sigStartNewBootDev(pBootDev->first);
			m_mapBootDev.erase(pBootDev->first);
			break;
		}
	}

	// for system halt
	for (std::map<QString, QString>::iterator pSysDev=m_mapSysDev.begin();
		pSysDev != m_mapSysDev.end(); ++pSysDev) {
		QString szData=QString().sprintf("ebxs:%s", QSZ(pSysDev->second));
		if (-1 == dataList.indexOf(szData)) {
			emit sigHaltSysDev(pSysDev->first);
			m_mapSysDev.erase(pSysDev->first);
			break;
		}
	}
	// for system
	for (data=dataList.begin(); data != dataList.end(); data++) {
		if (0 == data->left(4).compare(UBOOT_BCAST_PREFIX)) {
			QString szIp=data->mid(5);

			bool bOK=false;
			int nSubAddr=szIp.split('.').at(3).toInt(&bOK, 10);

			std::map<int, QString>::iterator findBoot =
					m_mapBootDev.find(nSubAddr);

			if (findBoot == m_mapBootDev.end()) {
				DMSG("add: %d", nSubAddr);
				m_mapBootDev[nSubAddr]=szIp;
			}
		}
		else if (0 == data->left(4).compare(SYS_BCAST_PREFIX)) {
			QStringList ipList=data->split(':');

			std::map<QString, QString>::iterator findSys =
					m_mapSysDev.find(ipList.at(2));

			if (findSys == m_mapSysDev.end()) {
				DMSG("add %s", QSZ(ipList.at(2)));
				m_mapSysDev[ipList.at(2)]=QString("%1:%2").arg(ipList.at(1)).arg(ipList.at(2));
				emit sigStartNewSysDev(QString("%1:%2").arg(ipList.at(1)).arg(ipList.at(2)));
			}
		}
	}
	m_szRecvData.clear();

	return 0;
}


int CNotifyRecv::pause() {
#ifdef Q_OS_WIN
	if (m_hThread) {
		::SuspendThread(m_hThread);
	}
	if (m_hParserThread) {
		::SuspendThread(m_hParserThread);
	}
#endif
	return 0;
}

int CNotifyRecv::resume() {
#ifdef Q_OS_WIN
	if (m_hThread) {
		::ResumeThread(m_hThread);
	}
	if (m_hParserThread) {
		::ResumeThread(m_hParserThread);
	}
#endif
	return 0;
}
