#include "csqlitestore.h"
#include "debug.h"
#include <QFile>
#include <QSqlError>
#include <QSqlRecord>

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

	m_db = QSqlDatabase::addDatabase("MYSQL");
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
	return true;
}

