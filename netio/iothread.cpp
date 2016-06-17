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
