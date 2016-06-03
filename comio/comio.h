#ifndef COMIO_H
#define COMIO_H

#include "comio_global.h"

#include "base_io.h"
#include <QtSerialPort>

class COMIOSHARED_EXPORT CComIO : public CBaseIO<QSerialPort*>
{
public:
	CComIO();
	virtual ~CComIO();

	virtual size_t write(unsigned char *data, size_t nLeng);
	virtual size_t write(const QString &data);
	virtual size_t read(unsigned char *data, size_t nLimit);
	virtual size_t read(QString &data);

	virtual bool open(char *sz);

	virtual void close();

	QMetaObject::Connection connect(const QObject *sender, const char *signal, const QObject *receiver, const char *method, Qt::ConnectionType type = Qt::AutoConnection) {
		return m_io->connect(sender, signal, receiver, method, type);
	}

	bool disconnect(const QObject *sender, const char *signal, const QObject *receiver, const char *method) {
		return m_io->disconnect(sender, signal, receiver, method);
	}
};

#endif // COMIO_H
