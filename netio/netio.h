#ifndef NETIO_H
#define NETIO_H

#include "netio_global.h"
#include "base_io.h"

#ifdef Q_OS_WIN
#include <winsock2.h>
#endif

#include <QThread>

class NETIOSHARED_EXPORT CIOThread : public QThread
{
	Q_OBJECT
private:
	CRootIO *m_io;
public:
	CIOThread();
	~CIOThread();

	void setIO(CRootIO *io);
	virtual void run();
};


class NETIOSHARED_EXPORT CNetcatIO : public CBaseIO<int>
{
private:
	bool isIpAddr(QString szIpAddr);

	bool openClient(QStringList &szList);
	bool openServer(QStringList &szList);

	size_t readSocket(char *data, size_t nLimit);

	sockaddr_in m_addr;
	int m_nPort;

	bool m_bIsServer;
	QString m_szRecvData;

protected:
	CIOThread *m_thread;

public:
	CNetcatIO();
	virtual ~CNetcatIO();

	virtual size_t write(char *data, size_t nLeng);
	virtual size_t write(const QString &data);
	virtual size_t read(char *data, size_t nLimit);
	virtual size_t read(QString &data);

	virtual bool open(char *sz);

	virtual void close();

	virtual int run();

	static int runThread(CNetcatIO *io);
};

#endif // NETIO_H
