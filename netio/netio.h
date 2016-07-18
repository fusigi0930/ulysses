#ifndef NETIO_H
#define NETIO_H

#include "netio_global.h"
#include "base_io.h"
#include "base_action.h"

#ifdef Q_OS_WIN
#include <winsock2.h>
#endif

#include <QThread>
#include <map>
#include <QMutex>

#define _WAIT_DONE		0
#define _WAIT_TIMEOUT	1

class NETIOSHARED_EXPORT CIOThread : public QThread
{
	Q_OBJECT
protected:
	CRootIO *m_io;
public:
	CIOThread();
	~CIOThread();

	virtual void setIO(CRootIO *io);
	virtual void run();
};

class NETIOSHARED_EXPORT CIOParserThread : public CIOThread
{
	Q_OBJECT

public:
	CIOParserThread();
	~CIOParserThread();

	virtual void run();
};

class NETIOSHARED_EXPORT CNetActionThread : public QThread
{
	Q_OBJECT
protected:
	CBaseAction *m_action;

#ifdef Q_OS_WIN
	HANDLE m_hThread;
#endif

signals:

public slots:

public:
	CNetActionThread();
	CNetActionThread(CBaseAction *action);

	~CNetActionThread();

	virtual void run();

	virtual int pause();
	virtual int resume();

};


class NETIOSHARED_EXPORT CNetcatIO : public QObject, public CBaseIO<int>
{
	Q_OBJECT
protected:
	bool isIpAddr(QString szIpAddr);

	virtual bool openClient(QStringList &szList);
	virtual bool openServer(QStringList &szList);

	virtual size_t readSocket(char *data, size_t nLimit);

	char m_readBuf[1024];
	QMutex m_mutex;

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

	virtual int pause();
	virtual int resume();

	virtual bool isOpened();

signals:
	void sigStartKernel(int nPort);

public slots:
	void slotStartKernel();

};

class NETIOSHARED_EXPORT CTelnetIO : public CNetcatIO
{
	Q_OBJECT
protected:
	virtual bool openClient(QStringList &szList);
public:
	CTelnetIO();
	virtual ~CTelnetIO();

	virtual bool open(char *sz);

	virtual size_t write(char *data, size_t nLeng);
	virtual size_t write(QString &szData);
};

class NETIOSHARED_EXPORT CNotifyRecv : public CNetcatIO
{
	Q_OBJECT
private:
	std::map<int, QString> m_mapBootDev;
	std::map<QString, QString> m_mapSysDev;
	int parseBroadcast();

public:
#ifdef Q_OS_WIN
	HANDLE m_hParserThread;
#endif
	CIOThread *m_parserThread;
	CNotifyRecv();
	virtual ~CNotifyRecv();

	virtual bool open();
	virtual int runParser();
	virtual void close();

	virtual int pause();
	virtual int resume();


signals:
	void sigStartNewBootDev(int nPort);
	void sigStartNewSysDev(QString szIP);
	void sigHaltSysDev(QString szIP);

public slots:

};

class NETIOSHARED_EXPORT CNcIO : public CNetcatIO {
	Q_OBJECT
protected:
	virtual bool openClient(QStringList &szList);

public:
	sockaddr_in m_raddr;
	int m_rio;
public:
	CNcIO();
	virtual ~CNcIO();

	void close();
	virtual size_t readSocket(char *data, size_t nLimit);
};

#endif // NETIO_H
