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

	m_io=::socket(AF_INET, SOCK_STREAM, 0);

	if (-1 == m_io) {
		return false;
	}
	m_addr.sin_family=AF_INET;
	m_addr.sin_port=::htons(szList.at(3).toInt());

	m_nPort=szList.at(3).toInt();

	::memset(&m_addr.sin_zero, 0, sizeof(m_addr.sin_zero));

	::connect(m_io, reinterpret_cast<SOCKADDR*>(&m_addr), sizeof(m_addr));
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
