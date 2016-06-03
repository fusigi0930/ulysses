#include "comio.h"

#define TEMP_BUF_SIZE 1024

CComIO::CComIO() : CBaseIO<QSerialPort*>() {
	m_io=new QSerialPort();
}

CComIO::~CComIO() {
	if (m_io) {
		delete m_io;
		m_io=NULL;
	}
}

size_t CComIO::write(unsigned char *data, size_t nLeng) {
	return 0;
}

size_t CComIO::write(const QString &data) {
	return write(reinterpret_cast<unsigned char*>(data.toUtf8().data()),
				 data.length());
}

size_t CComIO::read(unsigned char *data, size_t nLimit) {
	return 0;
}

size_t CComIO::read(QString &data) {
	unsigned char arBuf[TEMP_BUF_SIZE]={0};
	size_t nRet=read(arBuf, sizeof(arBuf)-1);
	data=reinterpret_cast<char*>(arBuf);
	return nRet;
}

bool CComIO::open(char *sz) {
	return false;
}

void CComIO::close() {

}
