#include "netio.h"
#include "debug.h"

#include <QRegExp>

#include <QDateTime>
#include <QTime>

#ifdef Q_OS_WIN
#include <winsock2.h>
#endif

#include <unistd.h>

#define UBOOT_BCAST_PREFIX "ebxb"
#define SYS_BCAST_PREFIX "ebxs"

///////////////////////////////////////////////////////
/// \brief CNotifyRecv
///

CNotifyRecv::CNotifyRecv() : CNetcatIO() {

}

CNotifyRecv::~CNotifyRecv() {

}

bool CNotifyRecv::open() {
	return CNetcatIO::open("net:server:7500");
}

void CNotifyRecv::close() {
	CNetcatIO::close();
}

int CNotifyRecv::run() {
#ifdef Q_OS_WIN
	m_hThread=::GetCurrentThread();
#endif
	char arBuf[512];
	while (-1 != m_io) {
		memset (arBuf, 0, sizeof(arBuf));
		if (-1 == readSocket(arBuf, sizeof(arBuf)-1))
			break;
		m_szRecvData.append(arBuf);
		parseBroadcast();
		QThread::msleep(2000);
	}
#ifdef Q_OS_WIN
	m_hThread=NULL;
#endif
	return 0;
}

int CNotifyRecv::parseBroadcast() {
	m_szRecvData.replace('\r', '\n');
	QStringList dataList=m_szRecvData.split('\n', QString::SkipEmptyParts);
	QStringList::iterator data;

	// for bootloader
	for (std::map<int, QString>::iterator pBootDev=m_mapBootDev.begin();
		 pBootDev != m_mapBootDev.end(); pBootDev++) {
		QString szData=QString("ebxb:%1").arg(pBootDev->second);
		if (-1 == dataList.indexOf(szData)) {
			emit sigStartNewBootDev(pBootDev->first);
			m_mapBootDev.erase(pBootDev->first);
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
				m_mapBootDev[nSubAddr]=szIp;
			}
		}
		else if (0 == data->left(4).compare(SYS_BCAST_PREFIX)) {
			emit sigStartNewSysDev(data->mid(5));
		}
	}

	return 0;
}
