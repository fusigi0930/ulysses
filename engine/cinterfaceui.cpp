#include "cinterfaceui.h"
#include "debug.h"

CInterfaceUi::CInterfaceUi(QObject *parent) : QObject(parent)
{
	m_engine=NULL;
	m_nTreeModelId=0;
}

CInterfaceUi::~CInterfaceUi() {
	clearModels();
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
