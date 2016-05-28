#include "netio.h"
#include "debug.h"

#define TEMP_BUF_SIZE 1024

CNetIO::CNetIO() : CBaseIO<QTcpSocket>() {
	m_io=new QTcpSocket(NULL);
	m_udpIo=new QUdpSocket(NULL);
}

CNetIO::~CNetIO() {
	if (m_io) {
		delete m_io;
		m_io=NULL;
	}
	if (m_udpIo) {
		delete m_udpIo;
		m_udpIo=NULL;
	}

	m_listBroadcast.clear();
}

size_t CNetIO::write(unsigned char *data, size_t nLeng) {
	return 0;
}

size_t CNetIO::write(const QString &data) {
	return write(reinterpret_cast<unsigned char*>(data.toUtf8().data()),
				 data.length());
}

size_t CNetIO::read(unsigned char *data, size_t nLimit) {
	return 0;
}

size_t CNetIO::read(QString &data) {
	unsigned char arBuf[TEMP_BUF_SIZE]={0};
	size_t nRet=read(arBuf, sizeof(arBuf)-1);
	data=reinterpret_cast<char*>(arBuf);
	return nRet;
}

bool CNetIO::isIpAddr(QString szIpAddr) {
	return true;
}

bool CNetIO::open(char *sz) {
	QString szOpen=sz;
	QStringList szList=szOpen.split(":");

	if (0 != szList.at(0).compare("net", Qt::CaseInsensitive)) {
		DMSG("open type is not net io type!");
		return false;
	}

	// detect ip or server
	if (0 == szList.at(1).compare("server", Qt::CaseInsensitive)) {

	}
	else if (isIpAddr(szList.at(1))) {

	}

	return true;
}

void CNetIO::close() {

}

void CNetIO::startBroadcast(int nPort) {
	if (-1 != m_listBroadcast.indexOf(nPort)) {
		return;
	}
	m_listBroadcast.push_back(nPort);

	if (1 != m_listBroadcast.length())
		return;

	// start broadcast thread
}

void CNetIO::stopBroadcast(int nPort) {
	int nPos=m_listBroadcast.indexOf(nPort);
	if (-1 == nPos)
		return;
	m_listBroadcast.removeAt(nPos);
	if (0 == m_listBroadcast.length()) {
		// stop broadcast thread
	}
}

void CNetIO::dataBroadcast() {
	// broadcast
	QByteArray data="broadcast::CNetIO";
	for (QList<int>::iterator pPort=m_listBroadcast.begin();
		 pPort != m_listBroadcast.end(); pPort++) {

		m_udpIo->writeDatagram(data.data(), data.size(), QHostAddress::Broadcast,
							   *pPort);
	}
}
