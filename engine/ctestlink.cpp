#include "ctestlink.h"

#define DB_HOST "192.168.48.2"
#define DB_NAME "testlinkdb"
#define DB_USER "ulysses"
#define DB_PASS "ulysses"

///////////////////////////////////////////////
/// \brief CTestLinkSuite::CTestLinkSuite
///

CTestLinkPlan::CTestLinkPlan() {
	m_bOpened=false;
}

CTestLinkPlan::~CTestLinkPlan() {

}

bool CTestLinkPlan::open(QString szFile) {
	m_szName=szFile;
	QString szBuf;
	szBuf.sprintf("%s::%s::%s::%s::%s", DB_HOST, DB_NAME, DB_USER, DB_PASS, QSZ(m_szName));
	if (!m_db.open(QSZ(szBuf))) {
		DMSG("open testlinkdb failed!");
		return false;
	}
	m_bOpened=true;
	return true;
}

void CTestLinkPlan::close() {
	m_db.close();
	m_bOpened=false;
}

void CTestLinkPlan::setDevName(QString szName) {
	m_szDev=szName;
}


////////////////////////////////////////////////////////////
/// \brief CTestLinkItem::CTestLinkItem
///

CTestLinkItem::CTestLinkItem() : CRootItem()
{
	m_db=NULL;

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
