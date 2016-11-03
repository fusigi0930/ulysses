#ifndef CINTERFACEUI_H
#define CINTERFACEUI_H

#include <QQmlApplicationEngine>
#include "ctestlink.h"
#include "netio.h"
#include <map>
#include <QVariant>

class CInterfaceUi;

enum ETHREAD_FUNC {
	_EFUNC_GET_PLANS,
};

class CTaskThread : public QThread {
	Q_OBJECT
protected:
	CInterfaceUi *m_ui;

signals:

public slots:

public:
	CTaskThread(CInterfaceUi *ui);
	int m_nFunc;
	QString m_szDevName;

	~CTaskThread();

	virtual void run();
};

class CInterfaceUi : public QObject
{
	Q_OBJECT
private:
	std::map<QString, CTestLinkReader*> m_mapReaders;
	int m_nTreeModelId;
	CNotifyRecv m_broadcastRecv;

public:
	CTestLinkReader m_reader;

	CInterfaceUi(QObject *parent = 0);
	virtual ~CInterfaceUi();

	QQmlApplicationEngine *m_engine;

	void setEngine(QQmlApplicationEngine *engine);


	Q_INVOKABLE void newDBReader(QString szName);
	Q_INVOKABLE void getTestPlan(QString szName);

	void clearReaders();
	void tfuncGetPlan(QString szName);

signals:
	void sigNewDev(QVariant item);
	void sigHeltDev(QVariant item);

	void sigAddPlan(QVariant item);

public slots:
	void slotNewDev(QString szIp);
	void slotHeltDev(QString szIp);
};

#endif // CINTERFACEUI_H
