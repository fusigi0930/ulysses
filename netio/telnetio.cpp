#include "netio.h"
#include "debug.h"

#include <QRegExp>

#include <QDateTime>
#include <QTime>

#ifdef Q_OS_WIN
#include <winsock2.h>
#endif

#include <unistd.h>

#define TEMP_BUF_SIZE 1024

////////////////////////////////////////////////////
/// \brief CTelnetIO
///

CTelnetIO::CTelnetIO() : CNetcatIO() {

}

CTelnetIO::~CTelnetIO() {

}

bool CTelnetIO::openClient(QStringList &szList) {
	// net:client:<ip>:<port>
	if (szList.count() < 4)
		return false;
	if (!isIpAddr(szList.at(2)))
		return false;
	close();

	m_io=::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (-1 == m_io) {
		return false;
	}
	m_addr.sin_family=AF_INET;
	m_nPort=szList.at(3).toInt();
	DMSG("open string: %s, %d", QSZ(szList.at(2)), m_nPort);
	m_addr.sin_port=::htons(m_nPort);


	::memset(&m_addr.sin_zero, 0, sizeof(m_addr.sin_zero));

	if (0 != ::connect(m_io, reinterpret_cast<SOCKADDR*>(&m_addr), sizeof(m_addr))) {
		return false;
	}
	m_bIsServer=false;

	return true;
}

bool CTelnetIO::open(char *sz) {
	// net:client:<ip>:<port>
	if (NULL == sz) {
		return false;
	}
	QString szBuf;
	szBuf.sprintf("net:client:%s:23", sz);
	return CNetcatIO::open(QSZ(szBuf));
}

size_t CTelnetIO::write(char *data, size_t nLeng) {
	m_mutex.lock();
	size_t nSize=static_cast<size_t>(::send(m_io, data, nLeng, 0));
	m_mutex.unlock();
	return nSize;
}

size_t CTelnetIO::write(QString &szData) {
	return CNetcatIO::write(szData);
}
