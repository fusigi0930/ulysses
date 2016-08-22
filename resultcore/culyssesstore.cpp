#include "csqlitestore.h"
#include "debug.h"
#include <QFile>
#include <QSqlError>
#include <QSqlRecord>

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

	m_db = QSqlDatabase::addDatabase("MYSQL");
}

CUlyStore::~CUlyStore()
{

}

long long CUlyStore::add(const QVariant &item) {
	m_mutex.lock();

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
