#ifndef CFACTORYACTION_H
#define CFACTORYACTION_H

#include "base_action.h"
#include "netio.h"
#include <map>

struct SRunDev {
	CRootIO *rio;
	CRootIO *wio;
	CNetActionThread *thread;
	CBaseAction *action;
};

class CFactoryAction : public QObject, public CBaseAction
{
	Q_OBJECT
protected:
	CNotifyRecv m_broadcastRecv;
	std::map<QString, SRunDev*> m_mapDevice;
	CNetActionThread m_actionThread;

	bool m_bIsRunning;

public:
	CFactoryAction();
	virtual ~CFactoryAction();

	int init();
	virtual int run();
	virtual void close();

signals:
	void sigStartNewBootDev(QString szIp, QString szMac);


public slots:
	void slotStartNewBootDev(int nPort);
	void slotEndBootDev(int nPort);
	void slotStartNewSysDev(QString ip);
	void slotHeltSysDev(QString ip);

	void slotClose();
};

#endif // CFACTORYACTION_H
