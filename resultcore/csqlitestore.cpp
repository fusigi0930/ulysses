#include "csqlitestore.h"
#include "debug.h"
#include <QFile>
#include <QSqlError>

CSQLiteStore::CSQLiteStore() : CBaseStore(),
	m_szFile()
{

}

CSQLiteStore::~CSQLiteStore() {
	close();
}

bool CSQLiteStore::open(char *szFile) {
	if (NULL == szFile) {
		return false;
	}
	bool bResult=false;

	m_szFile=szFile;
	m_db = QSqlDatabase::addDatabase("QSQLITE");
	m_db.setDatabaseName(m_szFile);

	if (!QFile::exists(m_szFile)) {
		bResult=m_db.open();
		if (bResult)
			initDB();
	}
	else {
		bResult=m_db.open();
	}

	return bResult;
}

// "type": <target>, <board>, <item>
// target type
// "mac": mac address
// "wonum": work number
// "unum": unit output number
// "board_name": target board full name
// "idate": start date
// "id": id
//
// board type
// "tid": target id
// "id": id
// "result": final test result
//
// item type
// "id":id
// "tid": target id
// "bid": board id
// "name": item name
// "result": test result
// "tdate": test date

long long CSQLiteStore::add(const QVariant &item) {
	QVariantMap mapItem=item.toMap();
	long long nRet=0;
	if (0 == mapItem["type"].toString().compare("target")) {
		nRet=addTarget(mapItem);
	}
	else if (0 == mapItem["type"].toString().compare("board")) {
		nRet=addBoard(mapItem);
	}
	else if (0 == mapItem["type"].toString().compare("item")) {
		nRet=addItem(mapItem);
	}
	return nRet;
}

bool CSQLiteStore::update(const QVariant &item) {
	QVariantMap mapItem=item.toMap();
	bool bRet=false;
	if (0 == mapItem["type"].toString().compare("target")) {
		bRet=updateTarget(mapItem);
	}
	else if (0 == mapItem["type"].toString().compare("board")) {
		bRet=updateBoard(mapItem);
	}
	else if (0 == mapItem["type"].toString().compare("item")) {
		bRet=updateItem(mapItem);
	}
	return false;
}

void CSQLiteStore::remove(const QVariant &item) {
	QVariantMap mapItem=item.toMap();
	if (0 == mapItem["type"].toString().compare("target")) {
		removeTarget(mapItem);
	}
	else if (0 == mapItem["type"].toString().compare("board")) {
		removeBoard(mapItem);
	}
	else if (0 == mapItem["type"].toString().compare("item")) {
		removeItem(mapItem);
	}
}

bool CSQLiteStore::query(std::list<QVariant> &result, char *fmt, ...) {
	return true;
}

void CSQLiteStore::close() {
	m_db.close();
}

bool CSQLiteStore::initDB() {
	// create target table
	m_db.exec(
		"create table target "
		"(id integer primary key autoincrement, "
		"mac text unique not null, "
		"wonum text unique not null, "
		"unum text unique, "
		"board_name text not null, "
		"sdate integer);"
		);

	if (QSqlError::NoError != m_db.lastError().type()) {
		return false;
	}

	// create index in target
	m_db.exec(
		"create index idx_target_mac "
		"on target (mac)"
		);

	if (QSqlError::NoError != m_db.lastError().type()) {
		return false;
	}

	// create board_info table
	m_db.exec(
		"create table board_info "
		"(id integer primary key autoincrement, "
		"tid integer not null, "
		"result integer, "
		"idate integer)"
		);

	if (QSqlError::NoError != m_db.lastError().type()) {
		return false;
	}

	// create index in board_info
	m_db.exec(
		"create index idx_binfo_tid "
		"on board_info (tid)"
		);

	if (QSqlError::NoError != m_db.lastError().type()) {
		return false;
	}

	// create item_info table
	m_db.exec(
		"create table item_info "
		"(id integer primary key autoincrement, "
		"tid integer not null, "
		"bid integer not null, "
		"name text not null, "
		"result integer, "
		"tdate integer)"
		);

	if (QSqlError::NoError != m_db.lastError().type()) {
		return false;
	}

	// create index in item_info
	m_db.exec(
		"create index idx_iinfo_tid "
		"on item_info (tid)"
		);

	if (QSqlError::NoError != m_db.lastError().type()) {
		return false;
	}

	m_db.exec(
		"create index idx_iinfo_bid "
		"on item_info (bid)"
		);

	if (QSqlError::NoError != m_db.lastError().type()) {
		return false;
	}

	return true;
}

long long CSQLiteStore::addTarget(const QVariantMap &item) {
	// target type
	// "mac": mac address
	// "wonum": work number
	// "unum": unit output number
	// "board_name": target board full name
	// "idate": start date
	// "id": id
	long long nRet=0;

	return nRet;
}

long long CSQLiteStore::addBoard(const QVariantMap &item) {
	// board type
	// "tid": target id
	// "id": id
	// "result": final test result
	long long nRet=0;

	return nRet;
}

long long CSQLiteStore::addItem(const QVariantMap &item) {
	// item type
	// "id":id
	// "tid": target id
	// "bid": board id
	// "name": item name
	// "result": test result
	// "tdate": test date
	long long nRet=0;

	return nRet;
}

bool CSQLiteStore::updateTarget(const QVariantMap &item) {
	// target type
	// "mac": mac address
	// "wonum": work number
	// "unum": unit output number
	// "board_name": target board full name
	// "idate": start date
	// "id": id
	bool bRet=false;

	return bRet;
}

bool CSQLiteStore::updateBoard(const QVariantMap &item) {
	// board type
	// "tid": target id
	// "id": id
	// "result": final test result
	//
	bool bRet=false;

	return bRet;
}

bool CSQLiteStore::updateItem(const QVariantMap &item) {
	// item type
	// "id":id
	// "tid": target id
	// "bid": board id
	// "name": item name
	// "result": test result
	// "tdate": test date
	bool bRet=false;

	return bRet;
}

void CSQLiteStore::removeTarget(const QVariantMap &item) {
	// target type
	// "mac": mac address
	// "wonum": work number
	// "unum": unit output number
	// "board_name": target board full name
	// "idate": start date
	// "id": id
}

void CSQLiteStore::removeBoard(const QVariantMap &item) {
	// board type
	// "tid": target id
	// "id": id
	// "result": final test result
}

void CSQLiteStore::removeItem(const QVariantMap &item) {
	// item type
	// "id":id
	// "tid": target id
	// "bid": board id
	// "name": item name
	// "result": test result
	// "tdate": test date
}
