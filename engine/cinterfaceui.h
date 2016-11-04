#ifndef CINTERFACEUI_H
#define CINTERFACEUI_H

#include <QQmlApplicationEngine>
#include "ctestlink.h"
#include "netio.h"
#include <map>
#include <QVariant>

#define _TTH_DEV_NAME "tlname"
#define _TTH_PROJID "projid"
#define _TTH_PLANID "planid"

class CInterfaceUi;

enum ETHREAD_FUNC {
	_EFUNC_GET_PLANS,
	_EFUNC_GET_TCS,
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
	QVariant m_vItem;

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
	Q_INVOKABLE void reqGetTC(QVariant item);

	void clearReaders();
	void tfuncGetPlan(QVariant item);
	void tfuncReqGetTC(QVariant item);

signals:
	void sigNewDev(QVariant item);
	void sigHeltDev(QVariant item);

	void sigAddPlan(QVariant item);

	void sigAddTC(QVariant item);

public slots:
	void slotNewDev(QString szIp);
	void slotHeltDev(QString szIp);
};

#endif // CINTERFACEUI_H
