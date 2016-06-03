#ifndef __BASE_IO_H__
#define __BASE_IO_H__

#include <QObject>
#include <QString>

/////////////////////////////////
/// open method parser
/// <method>:xxxxx:xxxx
/// every parameters should use the seprate character ":"
/// method: net, serial
/// for net:
/// the parameters should be:
///		parameter1:
///			ip:
///			server:
///		parameter2:
///			port:
/// for serial:
/// the paramters should be:
///		parameter1:
///			port name
///		parameter2:
///			baud rate
///		parameter3:
///			others
///

#define _IOTYPE_NET		0
#define _IOTYPE_SERAIL	1

class CRootIO {
public:
	CRootIO() {

	}

	virtual ~CRootIO() {

	}

    virtual size_t write(char *data, size_t nLeng) = 0;

	virtual size_t write(const QString &data) = 0;

    virtual size_t read(char *data, size_t nLimit) = 0;

	virtual size_t read(QString &data) = 0;

	virtual bool open(char *sz) = 0;

	virtual void close() = 0;
};

template <typename T_T>
class CBaseIO : public CRootIO {
protected:
    T_T m_io;

public:
	CBaseIO() : CRootIO() {

	}

	virtual ~CBaseIO() {

	}

    virtual size_t write(char *data, size_t nLeng) = 0;

	virtual size_t write(const QString &data) = 0;

    virtual size_t read(char *data, size_t nLimit) = 0;

	virtual size_t read(QString &data) = 0;

	virtual bool open(char *sz) = 0;

	virtual void close() = 0;

};
#endif // __BASE_IO_H__

