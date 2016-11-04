#include "cinterfaceui.h"
#include "debug.h"
#include <QVariantMap>
#include <algorithm>

//////////////////////////////////////////////
/// \brief CTaskThread::CTaskThread
/// \param ui
///

CTaskThread::CTaskThread(CInterfaceUi *ui) {
	m_ui=ui;
	m_nFunc=-1;
	DMSG("alloc address: 0x%x", this);
}

CTaskThread::~CTaskThread() {
	DMSG("delete address: 0x%x", this);
}

void CTaskThread::run() {
	if (!m_ui) return;

	switch (m_nFunc) {
		default: break;
		case _EFUNC_GET_PLANS:
			m_ui->tfuncGetPlan(m_vItem);
			break;
		case _EFUNC_GET_TCS:
			m_ui->tfuncReqGetTC(m_vItem);
			break;
	}

	deleteLater();
}

//////////////////////////////////////////////////
/// \brief CInterfaceUi::CInterfaceUi
/// \param parent
///

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
	clearReaders();
	m_broadcastRecv.close();
}

void CInterfaceUi::clearReaders() {
	for (std::map<QString, CTestLinkReader*>::iterator pItem=m_mapReaders.begin();
		pItem != m_mapReaders.end(); pItem++) {
		DMSG("delete address: 0x%x", pItem->second);
		pItem->second->close();
		delete pItem->second;
	}
	m_mapReaders.clear();
}

void CInterfaceUi::setEngine(QQmlApplicationEngine *engine) {
	m_engine=engine;
}

void CInterfaceUi::newDBReader(QString szName) {
	std::map<QString, CTestLinkReader*>::iterator pItem=m_mapReaders.find(szName);
	if (m_mapReaders.end() != pItem) return;

	CTestLinkReader *reader=new CTestLinkReader();
	DMSG("new address: 0x%x", reader);
	reader->setDevName(szName);
	reader->open(szName);
	m_mapReaders[szName]=reader;
}

void CInterfaceUi::getTestPlan(QString szName) {
	// the thread will be destroy after finished automaticlly
	CTaskThread	*task=new CTaskThread(this);
	DMSG("start update test plan thread!");
	task->m_nFunc=_EFUNC_GET_PLANS;
	QVariantMap mapItem;
	mapItem.insert(_TTH_DEV_NAME, szName);
	task->m_vItem=QVariant::fromValue(mapItem);

	task->start();
}

void CInterfaceUi::tfuncGetPlan(QVariant item) {
	QVariantMap mapItem=item.toMap();
	QString szName=mapItem[_TTH_DEV_NAME].toString();
	std::map<QString, CTestLinkReader*>::iterator pItem=m_mapReaders.find(szName);
	if (m_mapReaders.end() == pItem) return;

	pItem->second->getPlans();
	DMSG("update plan ui!");

	for (std::list<CTestLinkPlan*>::iterator pPlan=pItem->second->m_listPlans.begin();
		 pPlan != pItem->second->m_listPlans.end(); pPlan++) {

		DMSG("add test plan item!");
		QVariantMap mapItem;
		mapItem.insert("name", (*pPlan)->getName());
		mapItem.insert("objName", szName);
		mapItem.insert(_TTH_PLANID, (*pPlan)->m_nPlanId);
		mapItem.insert(_TTH_PROJID, (*pPlan)->getRoot()->m_nProjectId);
		mapItem.insert("enabled", true);
		emit sigAddPlan(QVariant::fromValue(mapItem));
	}
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

void CInterfaceUi::reqGetTC(QVariant item) {
	// the thread will be destroy after finished automaticlly
	CTaskThread	*task=new CTaskThread(this);
	DMSG("start update test case thread!");
	task->m_nFunc=_EFUNC_GET_TCS;
	task->m_vItem=item;
	task->start();
}

void CInterfaceUi::tfuncReqGetTC(QVariant item) {
	QVariantMap mapItem=item.toMap();
	QString szName=mapItem[_TTH_DEV_NAME].toString();
	std::map<QString, CTestLinkReader*>::iterator pItem=m_mapReaders.find(szName);
	if (m_mapReaders.end() == pItem) return;

	std::list<CTestLinkPlan*>::iterator pPlan;
	for (pPlan = pItem->second->m_listPlans.begin(); pPlan != pItem->second->m_listPlans.end(); pPlan++) {
		if ((*pPlan)->m_nPlanId == mapItem[_TTH_PLANID]) {
			DMSG("find plan!!");
			break;
		}
	}

	if (pPlan == pItem->second->m_listPlans.end()) return;

	(*pPlan)->getTCs();
	std::list<QVariant>::iterator pTC;
	for (pTC=(*pPlan)->m_tcs.begin(); pTC != (*pPlan)->m_tcs.end(); pTC++) {
		QVariantMap tcItem;
		tcItem.insert(_TTH_DEV_NAME, (*pPlan)->getRoot()->getDevName());
		tcItem.insert("id", pTC->toMap()["id"]);
		tcItem.insert("name", pTC->toMap()["name"]);
		tcItem.insert("pid", pTC->toMap()["parent_id"]);
		tcItem.insert("summary", pTC->toMap()["summary"]);
		tcItem.insert("result", "");
		tcItem.insert("enabled", true);

		emit sigAddTC(QVariant::fromValue(tcItem));
	}
}
