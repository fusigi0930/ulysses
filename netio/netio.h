#ifndef NETIO_H
#define NETIO_H

#include "netio_global.h"
#include "base_io.h"

#ifdef Q_OS_WIN
#include <winsock2.h>
#endif

#include <QThread>
#include <map>

#define _WAIT_DONE		0
#define _WAIT_TIMEOUT	1

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

class NETIOSHARED_EXPORT CNetActionThread : public QThread
{
	Q_OBJECT
signals:

public slots:

public:

};


class NETIOSHARED_EXPORT CNetcatIO : public QObject, public CBaseIO<int>
{
	Q_OBJECT
protected:
	bool isIpAddr(QString szIpAddr);

	virtual bool openClient(QStringList &szList);
	virtual bool openServer(QStringList &szList);

	size_t readSocket(char *data, size_t nLimit);

public:
	sockaddr_in m_addr;
	int m_nPort;

	bool m_bIsServer;
	QString m_szRecvData;

	CIOThread *m_thread;	

	QString m_szPrompt;

#ifdef Q_OS_WIN
	HANDLE m_hThread;
#endif

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
	virtual int setPrompt(QString szPrompt);
	virtual int waitPrompt(int nTimout);
#ifdef Q_OS_WIN
	virtual int pause();
	virtual int resume();
#endif
signals:
	void sigStartKernel();

public slots:
	void slotStartKernel();

};

class NETIOSHARED_EXPORT CTelnetIO : public CNetcatIO
{
	Q_OBJECT
public:
	CTelnetIO();
	virtual ~CTelnetIO();
};

class NETIOSHARED_EXPORT CNotifyRecv : public CNetcatIO
{
	Q_OBJECT
private:
	std::map<int, QString> m_mapBootDev;
	int parseBroadcast();

public:
	CNotifyRecv();
	virtual ~CNotifyRecv();

	virtual bool open();
	virtual int run();
	virtual void close();

signals:
	void sigStartNewBootDev(int nPort);
	void sigStartNewSysDev(QString szIP);

public slots:

};

#endif // NETIO_H
