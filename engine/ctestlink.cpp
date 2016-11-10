#include "ctestlink.h"
#include <QVariantMap>
#include <QVariantList>

#define DB_HOST "192.168.48.2"
#define DB_NAME "testlinkdb"
#define DB_USER "ulysses"
#define DB_PASS "ulysses"
#define DB_USER_ID 4

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

QVariant CTestLinkReader::fetchTCInfo(QVariant item) {
	// because the test case info only need the test case id
	// we get it in reader database!

	DMSG("get details tc info!!");
	std::list<QVariant> tcinfo;
	QVariantMap mapItem=item.toMap();

	m_db.query(tcinfo, "SELECT TS.id,TV.summary, TV.preconditions, TS.actions, TS.expected_results, TS.execution_type "
			   "FROM  `tcversions` TV,  `tcsteps` TS "
			   "WHERE TV.id "
			   "IN ( "
				   "SELECT id "
				   "FROM  `nodes_hierarchy` NH "
				   "WHERE NH.parent_id = %d "
			   ") "
			   "AND TS.id "
			   "IN ( "
				   "SELECT id "
				   "FROM  `nodes_hierarchy` NH "
				   "WHERE NH.parent_id = TV.id "
			   ");", mapItem["id"].toLongLong());

	QVariantList json;

	for (std::list<QVariant>::iterator pTC=tcinfo.begin(); pTC != tcinfo.end(); pTC++) {
		QVariantMap mapJson;
		mapJson.insert("tlname", mapItem["tlname"]);
		mapJson.insert("tcid", mapItem["id"]);
		mapJson.insert("summary", pTC->toMap()["summary"]);
		mapJson.insert("preconditions", pTC->toMap()["preconditions"]);
		mapJson.insert("actions", pTC->toMap()["actions"]);
		mapJson.insert("expected_results", pTC->toMap()["expected_results"]);
		mapJson.insert("execution_type", pTC->toMap()["execution_type"]);

		json.push_back(QVariant::fromValue(mapJson));
	}

	return QVariant::fromValue(json);
}

QVariant CTestLinkReader::fetchBuildInfo(QVariant item) {
	QVariantMap mapItem=item.toMap();

	long long nPlanId=mapItem[_TTH_PLANID].toLongLong();

	std::list<CTestLinkPlan*>::iterator pPlan;
	for (pPlan = m_listPlans.begin(); pPlan != m_listPlans.end(); pPlan++) {
		if ((*pPlan)->m_nPlanId == nPlanId) {
			break;
		}
	}

	if (pPlan == m_listPlans.end()) return QVariant();

	QVariantList vBuildInfo=(*pPlan)->fetchBuildInfo(item).toList();

	QVariantList buildInfo;

	for (int i=0; i < vBuildInfo.size(); i++) {
		buildInfo.push_back(vBuildInfo.at(i));
	}

	std::list<QVariant> listPlat;
	m_db.query(listPlat, "select * from platforms where testproject_id=%d;", m_nProjectId);
	for (std::list<QVariant>::iterator pInfo=listPlat.begin(); pInfo != listPlat.end(); pInfo++) {
		QVariantMap info=pInfo->toMap();
		info.insert("type", "platform");
		info.insert(_TTH_DEV_NAME, m_szName);
		buildInfo.push_back(QVariant::fromValue(info));
	}
	return QVariant::fromValue(buildInfo);
}

///////////////////////////////////////////////
/// \brief CTestLinkSuite::CTestLinkSuite
///

CTestLinkPlan::CTestLinkPlan() {
	m_db=NULL;
	m_nBuildId=-1;
	m_nPlanId=-1;
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
	this->m_nBuildId=plan->m_nBuildId;
}

QString CTestLinkPlan::getName() {
	return m_szName;
}

void CTestLinkPlan::getTCs() {
	m_tcs.clear();
	std::list<QVariant> tcs;
	// get tcversion_id
	m_db->query(tcs, "select * from `testplan_tcversions` where testplan_id=%d", m_nPlanId);

	for (std::list<QVariant>::iterator pTCVer=tcs.begin(); pTCVer != tcs.end(); pTCVer++) {
		long long nTCVerId=pTCVer->toMap()["tcversion_id"].toLongLong();
		// get current tc id
		std::list<QVariant> tcs_v, tcs_title, tcs_summary;

		m_db->query(tcs_v, "select * from `nodes_hierarchy` T1 where node_type_id=%d and id=%d",
							  _TL_NTYPE_TC_VERSION, nTCVerId);

		m_db->query(tcs_title, "select * from `nodes_hierarchy` T1 where id=%d",
							  tcs_v.begin()->toMap()["parent_id"].toLongLong());

		m_db->query(tcs_summary, "select * from `tcversions` T1 where id=%d",
							  tcs_v.begin()->toMap()["id"].toLongLong());

		QVariant tc=*tcs_title.begin();
		QVariantMap mapTC=tc.toMap();
		QString szBuf=tcs_summary.begin()->toMap()["summary"].toString();
		DMSG("summary: %s", QSZ(szBuf));
		mapTC.insert("summary", tcs_summary.begin()->toMap()["summary"].toString());

		m_tcs.push_back(QVariant::fromValue(mapTC));
	}
}

QVariant CTestLinkPlan::fetchBuildInfo(QVariant item) {
	if (!m_db) return QVariant();

	QVariantMap mapItem=item.toMap();
	QString szDev=mapItem[_TTH_DEV_NAME].toString();
	QStringList listDev=szDev.split(":");

	std::list<QVariant> listBuild;

	m_db->query(listBuild, "select * from builds where name like '%s%%' and testplan_id=%d;",
				QSZ(listDev.at(0)), m_nPlanId);

	QVariantList builds;
	for (std::list<QVariant>::iterator pBuild=listBuild.begin(); pBuild != listBuild.end(); pBuild++) {
		QVariantMap mapData=pBuild->toMap();
		mapData.insert("type", "build");
		mapData.insert(_TTH_DEV_NAME, szDev);
		builds.push_back(QVariant::fromValue(mapData));
	}

	return QVariant::fromValue(builds);
}

QVariant CTestLinkPlan::getNewestBuild() {
	if (!m_db) return QVariant();

	QVariantMap mapRet;
	std::list<QVariant> listBuf;
	m_db->query(listBuf, "select * from builds where testplan_id=%d order by id desc limit 1;", m_nPlanId);
	if (0 >= listBuf.size()) return QVariant();

	mapRet.insert("buildid", listBuf.begin()->toMap()["id"].toLongLong());

	listBuf.clear();
	m_db->query(listBuf, "select * from platforms where testproject_id=%d order by id desc limit 1;", m_nProjectId);
	if (0 >= listBuf.size()) return QVariant::fromValue(mapRet);

	mapRet.insert("platid", listBuf.begin()->toMap()["id"].toLongLong());

	return QVariant::fromValue(mapRet);
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

CTestLinkRoot* CTestLinkItem::getParent() {
	return m_parent;
}

void CTestLinkItem::duplicateInfo(CTestLinkRoot *item) {
	CTestLinkRoot::duplicateInfo(item);
	CTestLinkItem *citem=dynamic_cast<CTestLinkItem*>(item);
	if (NULL == item) return;


}
