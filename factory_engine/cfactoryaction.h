#ifndef CFACTORYACTION_H
#define CFACTORYACTION_H

#include "cdoaction.h"
#include "netio.h"
#include <map>
#include <QTimer>
#include <QVariant>
#include <csqlitestore.h>

class CFactoryAction : public QObject, public CBaseAction
{
	Q_OBJECT
	Q_PROPERTY(QString getXmlFile READ getXmlFile WRITE setXmlFile)

protected:
	CNotifyRecv m_broadcastRecv;
	std::map<QString, SRunDev*> m_mapDevice;
	CNetActionThread m_actionThread;

	bool m_bIsRunning;

	QTimer m_timer;

	void destoryDev(SRunDev *dev);

	QString m_szXmlName;

	CSQLiteStore m_db;

	void wait(int msec);
	long long processTarget(SRunDev *dev);
public:
	CFactoryAction();
	virtual ~CFactoryAction();

	int init();
	virtual int run();
	virtual void close();

	QString getXmlFile() {
		return m_szXmlName;
	}

	void setXmlFile(QString item) {
		m_szXmlName=item;
	}

signals:
	void sigStartNewBootDev(QString szIp, QString szMac);
	void sigUpdateHost(QVariant item);
	void sigRemoveHost(QVariant item);

	void sigAddShowItem(QVariant item);
	void sigUpdateShowItem(QVariant item);

public slots:
	void slotRemoveHost(QVariant item);
	void slotRemoveFailedHosts();

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
