#include "csqlitestore.h"
#include "debug.h"
#include <QFile>
#include <QSqlError>
#include <QSqlRecord>

#define DB_HOST "192.168.48.2"
#define DB_NAME "test_product_report"
#define DB_USER "fortress"
#define DB_PASS "fortress"

CDBSyncStore::CDBSyncStore()
{
	/*
	 * DB_TYPE: mysql
	 * DB_USER: root
	 * DB_PASS: coder@123
	 * DB_HOST: 192.168.48.2
	 * DB_NAME: test_product_report, product_report
	 * DB_TABLE_PREFIX:
	 *
	 */

}

CDBSyncStore::~CDBSyncStore()
{

}

long long CDBSyncStore::add(const QVariant &item) {
	m_mutex.lock();

	m_mutex.unlock();
	return 0;
}

bool CDBSyncStore::update(const QVariant &item) {
	m_mutex.lock();

	m_mutex.unlock();
	return true;
}

void CDBSyncStore::remove(const QVariant &item) {
	m_mutex.lock();

	m_mutex.unlock();
}

bool CDBSyncStore::initDB() {
	m_szDBName.sprintf("%s::%d::%s::%s");

	m_db = QSqlDatabase::addDatabase("QMYSQL", m_szDBName);

	if (m_db.isOpen()) {
		m_db.close();
	}

	m_db.setHostName(DB_HOST);
	m_db.setDatabaseName(DB_NAME);
	m_db.setUserName(DB_USER);
	m_db.setPassword(DB_PASS);

	if (!m_db.open()) {
		return false;
	}

	return true;
}


