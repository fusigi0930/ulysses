#ifndef CINTERFACEUI_H
#define CINTERFACEUI_H

#include <QQmlApplicationEngine>
#include "ctreemodel.h"
#include "netio.h"
#include <map>
#include <QVariant>

class CInterfaceUi : public QObject
{
	Q_OBJECT
private:
	std::map<QString, CTreeModel*> m_mapTreeModel;
	int m_nTreeModelId;
	CNotifyRecv m_broadcastRecv;

public:
	CInterfaceUi(QObject *parent = 0);
	virtual ~CInterfaceUi();

	QQmlApplicationEngine *m_engine;

	void setEngine(QQmlApplicationEngine *engine);


	Q_INVOKABLE QVariant newTreeModel();

	void clearModels();

signals:
	void sigNewDev(QVariant item);
	void sigHeltDev(QVariant item);

public slots:
	void slotNewDev(QString szIp);
	void slotHeltDev(QString szIp);
};

#endif // CINTERFACEUI_H
