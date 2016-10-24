#ifndef CINTERFACEUI_H
#define CINTERFACEUI_H

#include <QQmlApplicationEngine>
#include "ctreemodel.h"
#include <map>

class CInterfaceUi : public QObject
{
	Q_OBJECT
private:
	std::map<QString, CTreeModel*> m_mapTreeModel;
	int m_nTreeModelId;
public:
	CInterfaceUi(QObject *parent = 0);
	virtual ~CInterfaceUi();

	QQmlApplicationEngine *m_engine;

	void setEngine(QQmlApplicationEngine *engine);


	Q_INVOKABLE QVariant newTreeModel();

	void clearModels();

signals:

public slots:
};

#endif // CINTERFACEUI_H
