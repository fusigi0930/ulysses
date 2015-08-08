#ifndef __BASE_IO_H__
#define __BASE_IO_H__

#include <QObject>
#include <QString>

template <typename T_T>
class CBaseIO {
protected:
	T_T *m_io;

public:
	CBaseIO() {

	}

	virtual ~CBaseIO() {

	}

	virtual size_t write(unsigned char *data, size_t nLeng) = 0;

	virtual size_t read(unsigned char *data, size_t nLimit) = 0;

	virtual bool open(char *sz) = 0;

	virtual void close() = 0;
};

#endif // __BASE_IO_H__
