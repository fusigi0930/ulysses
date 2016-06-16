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
}

size_t CNetcatIO::write(char *data, size_t nLeng) {
	if (-1 == m_io) return 0;

	size_t nSize=static_cast<size_t>(::send(m_io, data, nLeng, 0));
	QString szData=data;
	if (szData.contains("boot") || szData.contains("run mmcboot") || szData.contains("run localboot") ||
			szData.contains("bootm")) {
		emit sigStartKernel();
	}
	return nSize;
}

size_t CNetcatIO::write(const QString &data) {
	return write(reinterpret_cast<char*>(data.toUtf8().data()),
				 data.length());
}

size_t CNetcatIO::readSocket(char *data, size_t nLimit) {
	if (-1 == m_io) return 0;

	size_t nSize=static_cast<size_t>(::recv(m_io, data, nLimit, 0));
	return nSize;
}

size_t CNetcatIO::read(char *data, size_t nLimit) {
	size_t nRet=0;
	// pause thread

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

	// resume thread

	return nRet;
}

size_t CNetcatIO::read(QString &data) {
	// pause thread

	size_t nRet=m_szRecvData.length();
	data=m_szRecvData;
	m_szRecvData.clear();

	// resume thread

	return nRet;
}

bool CNetcatIO::isIpAddr(QString szIpAddr) {
	QRegExp iprx("\\d{1,3}(\\.\\d{1,3}){3}");
	bool bResult=false;
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

	m_io=::socket(AF_INET, SOCK_DGRAM, 0);

	if (-1 == m_io) {
		return false;
	}
	m_addr.sin_family=AF_INET;
	m_addr.sin_port=::htons(szList.at(3).toInt());
	m_nPort=szList.at(3).toInt();

	::memset(&m_addr.sin_zero, 0, sizeof(m_addr.sin_zero));

	::connect(m_io, reinterpret_cast<SOCKADDR*>(&m_addr), sizeof(m_addr));
	m_bIsServer=false;

	connect(this, SIGNAL(sigStartKernel()), this, SLOT(slotStartKernel()), Qt::QueuedConnection);
	return true;
}

bool CNetcatIO::openServer(QStringList &szList) {
	// net:server:<port>
	if (szList.count() < 3)
		return false;
	close();

	m_io=::socket(AF_INET, SOCK_DGRAM, 0);
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

	::setsockopt(m_io, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<char*>(&option), sizeof(option));
	::setsockopt(m_io, SOL_SOCKET, SO_BROADCAST, reinterpret_cast<char*>(&option), sizeof(option));

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
	if (!m_bIsServer && -1 != m_io) {
		::shutdown(m_io, SD_SEND);
	}
	if (-1 != m_io) {
#ifdef Q_OS_WIN
		::closesocket(m_io);
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
	char arBuf[1024];
#ifdef Q_OS_WIN
	m_hThread=::GetCurrentThread();
#endif
	while (-1 != m_io) {
		memset (arBuf, 0, sizeof(arBuf));
		if (-1 == readSocket(arBuf, sizeof(arBuf)-1))
			break;
		m_szRecvData.append(arBuf);
	}
#ifdef Q_OS_WIN
	m_hThread=NULL;
#endif
	return 0;
}

int CNetcatIO::setPrompt(QString szPrompt) {
	m_szPrompt=szPrompt;
	return 0;
}

int CNetcatIO::waitPrompt(int nTimeout) {
	QRegExp prompt(m_szPrompt);
	QDateTime startTime=QDateTime::currentDateTime();
	while (nTimeout > (QDateTime::currentDateTime().toTime_t() - startTime.toTime_t())) {
		QString szData=m_szRecvData;
		szData.replace('\r', '\n');

		QStringList recvList=m_szRecvData.split("\n",  QString::SkipEmptyParts);
		if (0 < recvList.count()) {
			QString szLastLine=recvList.at(recvList.count()-1);
			if (szLastLine.contains(prompt))
				return _WAIT_DONE;
		}
		QThread::msleep(100);
	}
	return _WAIT_TIMEOUT;
}

#ifdef Q_OS_WIN
int CNetcatIO::pause() {
	if (m_hThread) {
		::SuspendThread(m_hThread);
	}
	return 0;
}

int CNetcatIO::resume() {
	if (m_hThread) {
		::ResumeThread(m_hThread);
	}
	return 0;
}

#endif

void CNetcatIO::slotStartKernel() {
	DMSG("start kernel, close connection");
	close();
}
