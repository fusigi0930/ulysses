#include "netio.h"
#include "debug.h"

CNetIO::CNetIO() : CBaseIO<QAbstractSocket>() {
	m_io=new QAbstractSocket(QAbstractSocket::TcpSocket, NULL);
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

size_t CNetIO::read(unsigned char *data, size_t nLimit) {
	return 0;
}

bool CNetIO::open(char *sz) {
	return false;
}

void CNetIO::close() {

}
