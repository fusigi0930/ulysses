#ifndef NETIO_H
#define NETIO_H

#include "netio_global.h"
#include "base_io.h"

#include <QAbstractSocket>

class NETIOSHARED_EXPORT CNetIO : public CBaseIO<QAbstractSocket>
{

public:
	CNetIO();
	virtual ~CNetIO();

	virtual size_t write(unsigned char *data, size_t nLeng) = 0;

	virtual size_t read(unsigned char *data, size_t nLimit) = 0;

	virtual bool open(char *sz) = 0;

	virtual void close() = 0;

	QMetaObject::Connection connect(const QObject *sender, const char *signal, const QObject *receiver, const char *method, Qt::ConnectionType type = Qt::AutoConnection) {
		return m_io->connect(sender, signal, receiver, method, type);
	}

	bool disconnect(const QObject *sender, const char *signal, const QObject *receiver, const char *method) {
		return m_io->disconnect(sender, signal, receiver, method);
	}
};

#endif // NETIO_H
