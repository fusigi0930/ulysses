#include "netio.h"
#include "debug.h"

#include <QRegExp>

#ifdef Q_OS_WIN
#include <winsock2.h>
#endif

#include <unistd.h>
#include <typeinfo>

#define TEMP_BUF_SIZE 1024

//////////////////////////////////////////////
/// CIOThread
///

CIOThread::CIOThread() : QThread() {
	m_io=NULL;
}

CIOThread::~CIOThread() {

}

void CIOThread::setIO(CRootIO *io) {
	m_io=io;
}

void CIOThread::run() {
	if (NULL == m_io)
		return;

	m_io->run();
}

//////////////////////////////////////////////
/// CIOParserThread
///

CIOParserThread::CIOParserThread() : CIOThread() {

}

CIOParserThread::~CIOParserThread() {

}

void CIOParserThread::run() {
	if (NULL == m_io) {
		DMSG("m_io is null");
		return;
	}
	CNotifyRecv *pRecv=dynamic_cast<CNotifyRecv*>(m_io);
	if (NULL == pRecv) {
		DMSG("type casting to CNotifyRecv failed!");
		return;
	}
	pRecv->runParser();
}

//////////////////////////////////////////////
/// CNetActionThread
///

CNetActionThread::CNetActionThread() : QThread() {
	m_action=NULL;
	m_hThread=NULL;
}

CNetActionThread::CNetActionThread(CBaseAction *action) : QThread() {
	m_action=action;
	m_hThread=NULL;
}

CNetActionThread::~CNetActionThread() {

}

void CNetActionThread::run() {
#ifdef Q_OS_WIN
	m_hThread=::GetCurrentThread();
#endif
	if (m_action) {
		m_action->run();
	}
#ifdef Q_OS_WIN
	m_hThread=NULL;
#endif
}

int CNetActionThread::pause() {
#ifdef Q_OS_WIN
	if (m_hThread) {
		::SuspendThread(m_hThread);
	}
#endif
	return 0;
}

int CNetActionThread::resume() {
#ifdef Q_OS_WIN
	if (m_hThread) {
		::ResumeThread(m_hThread);
	}
#endif
	return 0;
}
