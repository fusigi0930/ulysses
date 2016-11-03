#include "ctestlink.h"

#define DB_HOST "192.168.48.2"
#define DB_NAME "testlinkdb"
#define DB_USER "ulysses"
#define DB_PASS "ulysses"

//////////////////////////////////////////////
/// \brief CTestLinkReader::CTestLinkReader
///

CTestLinkReader::CTestLinkReader() {
	m_bIsOpened=false;

}

CTestLinkReader::~CTestLinkReader() {
	close();
}

void CTestLinkReader::close() {
	for (std::list<CTestLinkPlan*>::iterator pItem=m_listPlans.begin(); pItem != m_listPlans.end(); pItem++) {
		delete *pItem;
	}
	m_listPlans.clear();
	m_db.close();
	m_bIsOpened=false;
}

bool CTestLinkReader::open(QString szFile) {
	m_szName=szFile;
	QString szBuf;
	szBuf.sprintf("%s::%s::%s::%s::%s", DB_HOST, DB_NAME, DB_USER, DB_PASS, QSZ(m_szName));
	if (!m_db.open(QSZ(szBuf))) {
		DMSG("open testlinkdb failed: %s", QSZ(m_szName));
		return false;
	}
	m_bIsOpened=true;
	return true;
}

void CTestLinkReader::getPlans() {
	if (!m_bIsOpened) return;

	std::list<QVariant> list;

	m_db.query(list, "select * from `nodes_hierarchy` T1 where node_type_id=%d and name='embux';", _TL_NTYPE_TESTPROJ);
	if (0 >= list.size()) {
		DMSG("not get embux test project id!!!");
		return;
	}

	m_nProjectId=list.begin()->toMap()["id"].toLongLong();

	list.clear();
	m_db.query(list, "select * from `nodes_hierarchy` T1 where node_type_id=%d and parent_id=%lld and name like '%s%%';",
			   _TL_NTYPE_TESTPLAN, m_nProjectId, QSZ(m_szDev));

	for (std::list<QVariant>::iterator pItem=list.begin(); pItem!=list.end(); pItem++) {
		CTestLinkPlan *p=new CTestLinkPlan();
		p->setDB(&m_db);
		p->duplicateInfo(this);
		p->open(pItem->toMap()["name"].toString());
		p->m_nPlanId=pItem->toMap()["id"].toLongLong();
		m_listPlans.push_back(p);
	}

}

void CTestLinkReader::setDevName(QString szName) {
	m_szName=szName;
	m_szDev=szName.split(":").at(0);
	m_szIp=szName.split(":").at(1);
}

///////////////////////////////////////////////
/// \brief CTestLinkSuite::CTestLinkSuite
///

CTestLinkPlan::CTestLinkPlan() {
	m_parent=NULL;
	m_db=NULL;
}

CTestLinkPlan::~CTestLinkPlan() {

}

bool CTestLinkPlan::open(QString szFile) {
	m_szName=szFile;
	return true;
}

void CTestLinkPlan::close() {

}

void CTestLinkPlan::setDB(CUlyStore *db) {
	m_db=db;
}

CTestLinkRoot* CTestLinkPlan::getParent() {
	return m_parent;
}

void CTestLinkPlan::duplicateInfo(CTestLinkRoot *item) {
	CTestLinkRoot::duplicateInfo(item);
	CTestLinkPlan *plan=dynamic_cast<CTestLinkPlan*>(item);
	if (NULL == plan) return;

	this->m_nPlanId=plan->m_nPlanId;
}

QString CTestLinkPlan::getName() {
	return m_szName;
}

////////////////////////////////////////////////////////////
/// \brief CTestLinkItem::CTestLinkItem
///

CTestLinkItem::CTestLinkItem() : CRootItem()
{
	m_db=NULL;
	m_parent=NULL;
}

CTestLinkItem::~CTestLinkItem() {
	close();
}

void CTestLinkItem::setDB(CUlyStore *db) {
	m_db=db;
}

bool CTestLinkItem::open(QString szFile) {
	m_szName=szFile;
	return true;
}

bool CTestLinkItem::save(QString szFile) {
	return true;
}

void CTestLinkItem::close() {

}

bool CTestLinkItem::reload() {
	return true;
}

int CTestLinkItem::getItemCount() {
	return 0;
}

int CTestLinkItem::getCurrentIndex() {
	return 0;
}

void CTestLinkItem::setCurrentIndex(int nIndex) {

}

bool CTestLinkItem::nextItem() {
	return true;
}

bool CTestLinkItem::removeItem(int nIndex) {
	return false;
}

CTestLinkRoot* CTestLinkItem::getParent() {
	return m_parent;
}

void CTestLinkItem::duplicateInfo(CTestLinkRoot *item) {
	CTestLinkRoot::duplicateInfo(item);
	CTestLinkItem *citem=dynamic_cast<CTestLinkItem*>(item);
	if (NULL == item) return;


}
