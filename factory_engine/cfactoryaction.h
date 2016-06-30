#ifndef CFACTORYACTION_H
#define CFACTORYACTION_H

#include "cdoaction.h"
#include "netio.h"
#include <map>
#include <QTimer>
#include <QVariant>

class CFactoryAction : public QObject, public CBaseAction
{
	Q_OBJECT
protected:
	CNotifyRecv m_broadcastRecv;
	std::map<QString, SRunDev*> m_mapDevice;
	CNetActionThread m_actionThread;

	bool m_bIsRunning;

	QTimer m_timer;
public:
	CFactoryAction();
	virtual ~CFactoryAction();

	int init();
	virtual int run();
	virtual void close();

signals:
	void sigStartNewBootDev(QString szIp, QString szMac);
	void sigUpdateHost(QVariant item);
	void sigRemoveHost(QVariant item);

	void sigAddShowItem(QVariant item);

	void sigTest(int nPort);

public slots:
	void slotRemoveHost(QVariant item);

	void slotStartNewBootDev(int nPort);
	void slotEndBootDev(int nPort);
	void slotStartNewSysDev(QString ip);
	void slotHeltSysDev(QString ip);

	void slotShowItem(QString szIp);

	void slotClose();

	void slotTimerTimeout(QTimer *timer);
	void slotTimeoutClose(QTimer *timer);

};

#endif // CFACTORYACTION_H
