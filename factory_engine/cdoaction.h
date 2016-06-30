#ifndef CDOACTION_H
#define CDOACTION_H

#include "base_action.h"
#include "netio.h"
#include <QTimer>
#include "xmlcfg.h"
#include "xmlrun.h"

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
};

struct SRunDev {
	CRootIO *rio;
	CRootIO *wio;
	CRootIO *tio;
	CNetActionThread *thread;
	CBaseAction *action;
	CTimer *timer;
	QString szMac;
	QString szWo;
	QString szIp;
};

enum ERunStatus {
	_RUN_STATUS_BOOTLOADER,
	_RUN_STATUS_RAMDISK,
	_RUN_STATUS_UNKNOW = -1
};

class CDoAction : public QObject, public CBaseAction
{
	Q_OBJECT
private:
	int runBootCmd();
	int runSysCmd();

protected:
	SRunDev *m_ptrDev;
	CXmlFactoryRun m_xmlRun;
	CXmlConfig m_xmlConfig;

	QString m_szXmlFile;
	int m_nStatus;
public:
	CDoAction(SRunDev *dev);
	CDoAction();
	~CDoAction();

	virtual void close();
	virtual int run();

signals:
	void sigAddShowItem(QVariant item);

public slots:
	void slotSetXMLFile(QString szFile);
	void slotStartKernel(int status);

	void slotShowItems();
};

#endif // CDOACTION_H
