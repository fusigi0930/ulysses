#include "netio.h"
#include "debug.h"

#include <QRegExp>
#include <QtConcurrent/QtConcurrent>

#include <QDateTime>
#include <QTime>

#ifdef Q_OS_WIN
#include <winsock2.h>
#endif

#include <unistd.h>
#include <typeinfo>

#define TEMP_BUF_SIZE 1024

//////////////////////////////////////////////
/// CNetcatIO::CNetcatIO
///

CNetcatIO::CNetcatIO() : CBaseIO<int>() {
	m_io=-1;
	m_thread=NULL;
	m_bIsServer=false;
	m_hThread=NULL;
}

CNetcatIO::~CNetcatIO() {
	close();
#ifdef Q_OS_WIN
	WSACleanup();
#endif
}

size_t CNetcatIO::write(char *data, size_t nLeng) {
	if (-1 == m_io) return 0;

	m_mutex.lock();
	size_t nSize=static_cast<size_t>(::send(m_io, data, nLeng, 0));
	QString szData=data;
	if (szData.contains("boot") || szData.contains("run mmcboot") || szData.contains("run localboot") ||
			szData.contains("bootm")) {
		DMSG("emit sigStartKernel!!!!");
		emit sigStartKernel(m_nPort-7000);
	}
	m_mutex.unlock();
	return nSize;
}

size_t CNetcatIO::write(const QString &data) {
	return write(reinterpret_cast<char*>(data.toUtf8().data()),
				 data.length());
}

size_t CNetcatIO::readSocket(char *data, size_t nLimit) {
	if (-1 == m_io) return 0;

	size_t nSize=static_cast<size_t>(::recv(m_io, data, nLimit, 0));
#ifdef Q_OS_WIN
	if (nSize == SOCKET_ERROR) {
		DMSG("socket error: %d", WSAGetLastError());
		return 0;
	}
#endif
	return nSize;
}

size_t CNetcatIO::read(char *data, size_t nLimit) {
	size_t nRet=0;
	// pause thread

	m_mutex.lock();
	if (nLimit > m_szRecvData.length()) {
		memcpy(data, QSZ(m_szRecvData), m_szRecvData.length());
		nRet=m_szRecvData.length();
		m_szRecvData.clear();
	}
	else {
		memcpy(data, QSZ(m_szRecvData), nLimit);
		m_szRecvData.remove(0, nLimit);
		nRet=nLimit;
	}

	m_mutex.unlock();
	// resume thread

	return nRet;
}

size_t CNetcatIO::read(QString &data) {
	// pause thread

	m_mutex.lock();
	size_t nRet=m_szRecvData.length();
	data=m_szRecvData;
	m_szRecvData.clear();

	// resume thread

	m_mutex.unlock();
	return nRet;
}

bool CNetcatIO::isIpAddr(QString szIpAddr) {
	QRegExp iprx("\\d{1,3}(\\.\\d{1,3}){3}");
	bool bResult=false;
	hostent *theHost=NULL;

	theHost=::gethostbyname(QSZ(szIpAddr));
	if (NULL == theHost) {
		return false;
	}

	szIpAddr=::inet_ntoa(*reinterpret_cast<in_addr *>(*theHost->h_addr_list));

	if (0 == iprx.indexIn(szIpAddr)) {
		bResult=true;

		m_addr.sin_addr.s_addr = ::inet_addr(QSZ(szIpAddr));
	}
	return bResult;
}

bool CNetcatIO::openClient(QStringList &szList) {
	// net:client:<ip>:<port>
	if (szList.count() < 4)
		return false;
	if (!isIpAddr(szList.at(2)))
		return false;
	close();

	m_io=::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

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

bool CNetcatIO::openServer(QStringList &szList) {
	// net:server:<port>
	if (szList.count() < 3)
		return false;
	close();

	m_io=::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (-1 == m_io) {
		return false;
	}
	m_addr.sin_family=AF_INET;
	m_addr.sin_port=::htons(szList.at(2).toInt());
	m_nPort=szList.at(2).toInt();
	if (!isIpAddr("0.0.0.0"))
		return false;
	::memset(&m_addr.sin_zero, 0, sizeof(m_addr.sin_zero));

	::bind(m_io, reinterpret_cast<sockaddr*>(&m_addr), sizeof(m_addr));
	m_bIsServer=true;

	// timeout settings
#if 0
	fd_set rfd;
	FD_ZERO(&rfd);
	timeval timeout;
	timeout.tv_usec=0;
	timeout.tv_sec=1;
	FD_SET(m_io, &rfd);
	select(m_io+1, &rfd, NULL, NULL, &timeout);
#endif
	return true;
}

bool CNetcatIO::open(char *sz) {
	QString szOpen=sz;
	QStringList szList=szOpen.split(":");

#ifdef Q_OS_WIN
	WSADATA wsa;
	WSAStartup(MAKEWORD(2,2),&wsa);
#endif

	if (0 != szList.at(0).compare("net", Qt::CaseInsensitive)) {
		DMSG("open type is not net io type!");
		return false;
	}

	// detect ip or server
	if (0 == szList.at(1).compare("server", Qt::CaseInsensitive)) {
		if (!openServer(szList))
			return false;
	}
	else if (0 == szList.at(1).compare("client", Qt::CaseInsensitive)) {
		if (!openClient(szList))
			return false;
	}

	int option=1;

	if (typeid(*this) != typeid(CTelnetIO)) {
		::setsockopt(m_io, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<char*>(&option), sizeof(option));
		::setsockopt(m_io, SOL_SOCKET, SO_BROADCAST, reinterpret_cast<char*>(&option), sizeof(option));
	}
	else {
		DMSG("telnetio: open: %s", sz);
	}

	// start read thread
	if(NULL != m_thread) {
		m_thread->terminate();
		delete m_thread;
	}

	m_thread = new CIOThread;
	m_thread->setIO(this);
	m_thread->start();

	return true;
}

void CNetcatIO::close() {
#if 0
	if (!m_bIsServer && -1 != m_io) {
		::shutdown(m_io, SD_SEND);
	}
#endif
	if (-1 != m_io) {
#ifdef Q_OS_WIN
		::closesocket(m_io);
		DMSG("close socket %s", typeid(*this).name());
#else
		::close(m_io);
#endif
		m_io=-1;
	}

	if (NULL != m_thread) {
		if (m_thread->isRunning())
			m_thread->terminate();
		delete m_thread;
		m_thread=NULL;
	}

	m_hThread=NULL;
}

int CNetcatIO::run() {
	// avoid optimized
	m_readBuf[1020]='\n';
#ifdef Q_OS_WIN
	m_hThread=::GetCurrentThread();
#endif
	QThread::msleep(600);
	while (-1 != m_io) {
		memset (m_readBuf, 0, sizeof(m_readBuf));
		if (0 == readSocket(m_readBuf, sizeof(m_readBuf)-1)) {
#ifdef Q_OS_WIN
			DMSG("~!!!!!!!!!!! %s, %d", typeid(*this).name(), WSAGetLastError());
#endif
			break;
		}
		m_mutex.lock();
		m_szRecvData.append(m_readBuf);
		if (7500 == m_nPort) {
			//DMSG("data: %s", m_readBuf);
			m_szRecvData.append("\n");
		}
		m_mutex.unlock();
	}
#ifdef Q_OS_WIN
	m_hThread=NULL;
#endif

	if (typeid(*this) == typeid(CTelnetIO)) {
		fprintf(stderr, "%d\n", m_readBuf[0]);
	}

	return 0;
}

int CNetcatIO::setPrompt(QString szPrompt) {
	m_szPrompt=szPrompt;
	return 0;
}

int CNetcatIO::waitPrompt(int nTimeout) {
	QRegExp prompt(m_szPrompt);
	QDateTime startTime=QDateTime::currentDateTime();
	QString szData;
	while (nTimeout > (QDateTime::currentDateTime().toMSecsSinceEpoch() - startTime.toMSecsSinceEpoch())) {
		szData=m_szRecvData;
		szData.replace('\r', '\n');

		QStringList recvList=m_szRecvData.split("\n",  QString::SkipEmptyParts);
		if (1 < recvList.count()) {
			QString szLastLine=recvList.at(recvList.count()-1);
			if (szLastLine.contains(prompt)) {
				//DMSG("Wait the prompt!!");
				return _WAIT_DONE;
			}
		}
		QThread::msleep(100);
	}
	return _WAIT_TIMEOUT;
}


int CNetcatIO::pause() {
#ifdef Q_OS_WIN
	if (m_hThread) {
		::SuspendThread(m_hThread);
	}
#endif
	return 0;
}

int CNetcatIO::resume() {
#ifdef Q_OS_WIN
	if (m_hThread) {
		::ResumeThread(m_hThread);
	}
#endif
	return 0;
}

void CNetcatIO::slotStartKernel() {
	DMSG("start kernel, close connection");
	//close();
}

bool CNetcatIO::isOpened() {
	return (0 < m_io ? true : false);
}

CNcIO::CNcIO() : CNetcatIO() {
	m_rio=-1;
}

CNcIO::~CNcIO() {
}

void CNcIO::close() {
	if (-1 != m_rio) {
#ifdef Q_OS_WIN
		::closesocket(m_rio);
		DMSG("close socket %s", typeid(*this).name());
#else
		::close(m_wio);
#endif
		m_rio=-1;
	}
	CNetcatIO::close();
}

bool CNcIO::openClient(QStringList &szList) {
	if (!CNetcatIO::openClient(szList)) {
		return false;
	}

	m_rio=::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (-1 == m_rio) {
		return false;
	}

	QThread::msleep(150);

	m_raddr.sin_family=AF_INET;
	m_raddr.sin_port=::htons(m_nPort);
	m_raddr.sin_addr.s_addr = htonl(INADDR_ANY);
	::memset(&m_raddr.sin_zero, 0, sizeof(m_raddr.sin_zero));

	long option=1;
	if (0 != ::bind(m_rio, reinterpret_cast<sockaddr*>(&m_raddr), sizeof(m_raddr))) {
		DMSG("bind failed!!");
		return false;
	}
	::setsockopt(m_rio, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<char*>(&option), sizeof(option));
	::setsockopt(m_rio, SOL_SOCKET, SO_BROADCAST, reinterpret_cast<char*>(&option), sizeof(option));
	return true;
}

size_t CNcIO::readSocket(char *data, size_t nLimit) {
	if (-1 == m_rio) return 0;
	size_t nSize=static_cast<size_t>(::recv(m_rio, data, nLimit, 0));
#ifdef Q_OS_WIN
	if (nSize == SOCKET_ERROR) {
		DMSG("socket error: %d", WSAGetLastError());
		return 0;
	}
#endif
	return nSize;
}
