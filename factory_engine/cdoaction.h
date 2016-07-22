#ifndef CDOACTION_H
#define CDOACTION_H

#include "base_action.h"
#include "netio.h"
#include <QTimer>
#include "xmlcfg.h"
#include "xmlrun.h"
#include "csqlitestore.h"
#include <QMutex>

class CTimer : public QTimer {
	Q_OBJECT
public:
	CTimer();
	virtual ~CTimer();

signals:
	void sigTimeout(QTimer *timer);
	void sigTimeoutClose(QTimer *timer);

private slots:
	void slotTimeout();
	void slotTimeoutClose();
};

enum ETEST_STATUS {
	_TS_RUNNING,
	_TS_PASS,
	_TS_FAIL,
	_TS_ALREADY_TESTED,
};

enum ERUNTIME_STATUS {
	_RT_STANDBY,
	_RT_PROCESSING,
};

struct SDevDBInfo {
	long long nTargetID;
	long long nBoardID;
	qint64 nTime;
};

struct SRunDev {
	CRootIO *ncio;
	CRootIO *tio;
	CNetActionThread *thread;
	CBaseAction *action;
	CTimer *timer;
	QString szMac;
	QString szWo;
	QString szIp;
	SDevDBInfo info;
	int nStatus;
	int nRuntimeStatus;
};

enum ERunStatus {
	_RUN_STATUS_BOOTLOADER,
	_RUN_STATUS_RAMDISK,
	_RUN_STATUS_LAST
};

class CDoAction : public QObject, public CBaseAction
{
	Q_OBJECT
private:
	typedef int (CDoAction::*runFunc)(SFactoryItem *);

	int runBootCmd(SFactoryItem *item);
	int runSysCmd(SFactoryItem *item);

	int runPrePostCmd(QString szCmd);
	int runFinalAlarm(bool bResult);

	void init();

protected:
	SRunDev *m_ptrDev;
	CXmlFactoryRun m_xmlRun;
	CXmlConfig m_xmlConfig;

	QString m_szXmlFile;
	CBaseStore *m_pDB;
	int m_nStatus;

	QMutex m_mutex;
public:
	CDoAction(SRunDev *dev);
	CDoAction(SRunDev *dev, QString item);
	CDoAction();
	~CDoAction();

	virtual void close();
	virtual int run();

	virtual void actionBlock();
	virtual void actionUnblock();

	void setDB(CBaseStore *ptr);

signals:
	void sigAddShowItem(QVariant item);
	void sigUpdateShowItem(QVariant item);
	void sigUpdateHost(QVariant item);
	void sigStartKernel(int status);

public slots:
	void slotSetXMLFile(QString szFile);
	void slotStartKernel(int status);

	void slotShowItems();
};

#endif // CDOACTION_H
