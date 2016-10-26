#include "cinterfaceui.h"
#include "debug.h"
#include <QVariantMap>

CInterfaceUi::CInterfaceUi(QObject *parent) : QObject(parent),
	m_broadcastRecv()
{
	m_engine=NULL;
	m_nTreeModelId=0;

	connect(&m_broadcastRecv, SIGNAL(sigStartNewSysDev(QString)), this, SLOT(slotNewDev(QString)), Qt::QueuedConnection);
	connect(&m_broadcastRecv, SIGNAL(sigHaltSysDev(QString)), this, SLOT(slotHeltDev(QString)), Qt::QueuedConnection);

	m_broadcastRecv.open();
}

CInterfaceUi::~CInterfaceUi() {
	clearModels();
	m_broadcastRecv.close();
}

void CInterfaceUi::clearModels() {
	for (std::map<QString, CTreeModel*>::iterator pItem=m_mapTreeModel.begin();
		 pItem != m_mapTreeModel.end(); pItem++) {
		DMSG("delete address: 0x%x", pItem->second);
		delete pItem->second;
	}
	m_mapTreeModel.clear();
}

void CInterfaceUi::setEngine(QQmlApplicationEngine *engine) {
	m_engine=engine;
}

QVariant CInterfaceUi::newTreeModel() {
	CTreeModel *model = new CTreeModel();
	DMSG("new address: 0x%x", model);
	QString szName;
	szName.sprintf("treeModel-%05d", m_nTreeModelId++);
	model->setQMLName(szName);
	m_mapTreeModel[szName]=model;
	return QVariant::fromValue(model);
}

void CInterfaceUi::slotNewDev(QString szIp) {
	QStringList listData=szIp.split(":");

	QString szType=listData.at(0);
	if (0 == szType.compare("factory")) {
		DMSG("need not process facotry code in thor hammer!!!");
		return;
	}
	szIp=listData.at(1);
	//{"mac":szMac,"ip":szIp,"style":"boot","itemcolor":"#C0C0E0"}

	QVariantMap item;
	item.insert("mac", "SystemTest");
	item.insert("ip", szIp);
	item.insert("style", szType);
	item.insert("itemcolor", "#C0C0E0");

	emit sigNewDev(QVariant::fromValue(item));
}

void CInterfaceUi::slotHeltDev(QString szIp) {
	//{"mac":szMac,"ip":szIp,"style":"boot","itemcolor":"#C0C0E0"}

	QVariantMap item;
	item.insert("ip", szIp);

	emit sigHeltDev(QVariant::fromValue(item));
}
