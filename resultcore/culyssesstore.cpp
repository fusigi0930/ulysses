#include "csqlitestore.h"
#include "debug.h"
#include <QFile>
#include <QSqlError>
#include <QSqlRecord>

#define DB_HOST "192.168.48.2"
#define DB_NAME "testlinkdb"
#define DB_USER "ulysses"
#define DB_PASS "ulysses"

CUlyStore::CUlyStore()
{
	/*
	 * DB_TYPE: mysql
	 * DB_USER: root
	 * DB_PASS: coder@123
	 * DB_HOST: 192.168.48.2
	 * DB_NAME: testlinkdb
	 * DB_TABLE_PREFIX:
	 *
	 */
}

CUlyStore::~CUlyStore()
{

}

long long CUlyStore::add(const QVariant &item) {
	m_mutex.lock();
	QVariantMap mapItem=item.toMap();
	long long nRet=0;
	if (0 == mapItem["type"].toString().compare("tc_result")) {

	}
	else if (0 == mapItem["type"].toString().compare("ts_result")) {

	}
	m_mutex.unlock();
	return 0;
}

bool CUlyStore::update(const QVariant &item) {
	m_mutex.lock();

	m_mutex.unlock();
	return true;
}

void CUlyStore::remove(const QVariant &item) {
	m_mutex.lock();

	m_mutex.unlock();
}

/*
 * read testlink data from db
 * open db project: testlinkdb
 * 1. get node types: select * from node_types
 *		1: testproject
 *		2: testsuite
 *		3: testcase
 *		4: testversion
 * .....
 * .....
 * 2. get main nodes: select * from nodes_hierarchy
 * 3. scenario:
 *		get all node is testcase node:
 *			select * from `node_types` T1,`nodes_hierarchy` T2 where T1.id=T2.node_type_id and T1.id=3
 *		test result: select * from `execution_tcsteps`
 *		get test step: select * from `tcsteps`
 *		get test summary and precondition:
 *			select * from `tcversions`
 *
 *		get test case detail info
 *			SELECT TS.id,TV.summary, TV.preconditions, TS.actions, TS.expected_results
 *			FROM  `tcversions` TV,  `tcsteps` TS
 *			WHERE TV.id
 *			IN (
 *				SELECT id
 *				FROM  `nodes_hierarchy` NH
 *				WHERE NH.parent_id =9
 *			)
 *			AND TS.id
 *			IN (
 *				SELECT id
 *				FROM  `nodes_hierarchy` NH
 *				WHERE NH.parent_id = TV.id
 *			)
 */
bool CUlyStore::initDB() {
	return true;
}

bool CUlyStore::open(char *szFile) {
	// szFile replace to open data base string
	// format:
	// <ip>::<database>::<user>::<password>
	if (NULL == szFile) {
		return initDB();
	}
	m_szDBName=szFile;

	m_db = QSqlDatabase::addDatabase("QMYSQL", m_szDBName);

	QStringList initCmd=QString(szFile).split("::");
	if (4 != initCmd.size()) {
		return false;
	}

	if (m_db.isOpen()) {
		m_db.close();
	}

	m_db.setHostName(initCmd.at(0));
	m_db.setDatabaseName(initCmd.at(1));
	m_db.setUserName(initCmd.at(2));
	m_db.setPassword(initCmd.at(3));

	if (!m_db.open()) {
		return false;
	}

	return true;
}

void CUlyStore::close() {
	m_db.close();
	QSqlDatabase::removeDatabase(m_szDBName);
}

void CUlyStore::insertItemMap(QVariantMap &item, QSqlRecord &rinfo, QSqlQuery &query, int nIndex, int nCount) {
	QString szField;
	if (0 == nCount) szField=rinfo.fieldName(nIndex);
	else szField.sprintf("%s.%d", QSZ(rinfo.fieldName(nIndex)), nCount);
	//DMSG("+++ recursive count: %d", nCount);
	if (item.find(szField) == item.end()) {
		item.insert(szField, query.value(nIndex));
	}
	else {
		insertItemMap(item, rinfo, query, nIndex, nCount+1);
	}
	//DMSG("--- recursive count: %d", nCount);
}

bool CUlyStore::query(QSqlQuery &q, char *fmt, ...) {
	q=m_db.exec();

	va_list vlist;
	va_start (vlist, fmt);
	char szBuf[1024]={0};
	vsprintf(szBuf, fmt, vlist);
	va_end(vlist);
	return q.exec(szBuf);
}

bool CUlyStore::query(std::list<QVariant> &result, char *fmt, ...) {
	result.clear();
	DMSG("get a query object");
	QSqlQuery query=m_db.exec();

	char szBuf[1024]={0};

	DMSG("process query command: %s", fmt);
	va_list vlist;
	va_start (vlist, fmt);
#ifdef Q_OS_WIN
	vsprintf_s(szBuf, sizeof(szBuf)-1, fmt, vlist);
#else
	vsprintf(szBuf, fmt, vlist);
#endif
	va_end(vlist);

	DMSG("query command: %s", szBuf);
	bool bRet=query.exec(szBuf);

	if (!bRet) {
		DMSG("exec failed!");
		return false;
	}

	DMSG("move records: %d to list contianer", query.size());
	QSqlRecord rinfo=query.record();
	while (query.next()) {
		QVariantMap item;
		for (int i=0; i < rinfo.count(); i++) {
			insertItemMap(item, rinfo, query, i);
		}
		result.push_back(QVariant::fromValue(item));
	}

	return bRet;
}
