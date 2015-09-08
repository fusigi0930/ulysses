#include "netio.h"
#include "debug.h"

#define TEMP_BUF_SIZE 1024

CNetIO::CNetIO() : CBaseIO<QTcpSocket>() {
	m_io=new QTcpSocket(NULL);
}

CNetIO::~CNetIO() {
	if (m_io) {
		delete m_io;
		m_io=NULL;
	}
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

bool CNetIO::open(char *sz) {
	return false;
}

void CNetIO::close() {

}
