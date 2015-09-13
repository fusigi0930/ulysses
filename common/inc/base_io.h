#ifndef __BASE_IO_H__
#define __BASE_IO_H__

#include <QObject>
#include <QString>

class CRootIO {
public:
	CRootIO() {

	}

	virtual ~CRootIO() {

	}

	virtual size_t write(unsigned char *data, size_t nLeng) = 0;

	virtual size_t write(const QString &data) = 0;

	virtual size_t read(unsigned char *data, size_t nLimit) = 0;

	virtual size_t read(QString &data) = 0;

	virtual bool open(char *sz) = 0;

	virtual void close() = 0;
};

template <typename T_T>
class CBaseIO : public CRootIO {
protected:
	T_T *m_io;

public:
	CBaseIO() : CRootIO() {

	}

	virtual ~CBaseIO() {

	}

	virtual size_t write(unsigned char *data, size_t nLeng) = 0;

	virtual size_t write(const QString &data) = 0;

	virtual size_t read(unsigned char *data, size_t nLimit) = 0;

	virtual size_t read(QString &data) = 0;

	virtual bool open(char *sz) = 0;

	virtual void close() = 0;

	virtual QMetaObject::Connection connect(const QObject *sender, const char *signal, const QObject *receiver, const char *method, Qt::ConnectionType type = Qt::AutoConnection) = 0;

	virtual bool disconnect(const QObject *sender, const char *signal, const QObject *receiver, const char *method) = 0;
};

#endif // __BASE_IO_H__

