#include "csqlitestore.h"
#include "debug.h"
#include <QFile>
#include <QSqlError>
#include <QSqlRecord>
#include <QNetworkInterface>

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
	else if (0 == mapItem["type"].toString().compare("sync")) {
		nRet=addSync(mapItem);
	}
	m_mutex.unlock();
	return nRet;
}

bool CDBSyncStore::update(const QVariant &item) {
	m_mutex.lock();
	QVariantMap mapItem=item.toMap();
	bool bRet=false;
	if (0 == mapItem["type"].toString().compare("sync")) {
		bRet=updateSync(mapItem);
	}
	m_mutex.unlock();
	return bRet;
}

bool CDBSyncStore::initDB() {
	m_szDBName.sprintf("%s::%s::%s::%s", DB_HOST, DB_NAME, DB_USER, DB_PASS);

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

long long CDBSyncStore::addTarget(const QVariantMap &item) {
	// target type
	// "mac": mac address
	// "wonum": work number
	// "unum": unit output number
	// "board_name": target board full name
	// "sdate": start date
	// "id": id
	long long nRet=0;
	QSqlQuery query=m_db.exec();

	query.prepare("insert into target (mac,wonum,board_name,unum,sdate) "
				  "values (?,?,?,?,?);"
				  );

	query.bindValue(0, item["mac"]);
	query.bindValue(1, item["wonum"]);
	query.bindValue(2, item["board_name"]);
	query.bindValue(3, item["unum"]);
	query.bindValue(4, item["sdate"]);

	query.exec();
	if (QSqlError::NoError != query.lastError().type()) {
		return nRet;
	}

	nRet=query.lastInsertId().toLongLong();

	return nRet;
}

long long CDBSyncStore::addBoard(const QVariantMap &item) {
	// board type
	// "tid": target id
	// "id": id
	// "result": final test result
	long long nRet=0;

	QSqlQuery query=m_db.exec();

	query.prepare("insert into board_info (tid,mode,result,idate) "
				  "values (?,?,?,?);"
				  );

	query.bindValue(0, item["tid"]);
	query.bindValue(1, item["mode"]);
	query.bindValue(2, item["result"]);
	query.bindValue(3, item["idate"]);

	query.exec();
	if (QSqlError::NoError != query.lastError().type()) {
		return nRet;
	}

	nRet=query.lastInsertId().toLongLong();

	return nRet;
}

long long CDBSyncStore::addItem(const QVariantMap &item) {
	// item type
	// "id":id
	// "tid": target id
	// "bid": board id
	// "name": item name
	// "result": test result
	// "tdate": test date
	long long nRet=0;

	QSqlQuery query=m_db.exec();

	query.prepare("insert into item_info (tid,bid,name,tdate,result) "
				  "values (?,?,?,?,?);"
				  );
	query.bindValue(0, item["tid"]);
	query.bindValue(1, item["bid"]);
	query.bindValue(2, item["name"]);
	query.bindValue(3, item["tdate"]);
	query.bindValue(4, item["result"]);

	query.exec();
	if (QSqlError::NoError != query.lastError().type()) {
		return nRet;
	}

	nRet=query.lastInsertId().toLongLong();

	return nRet;
}

long long CDBSyncStore::addSync(const QVariantMap &item) {
	// sync type
	// "id":id
	// "sid": server id
	// "sdate": sync time
	// "type": sync type
	long long nRet=0;

	QSqlQuery query=m_db.exec();

	query.prepare("insert into sync_info (`sdate`,`type`,`from`) "
				  "values (?,?,?);"
				  );

	query.bindValue(0, item["sdate"]);
	query.bindValue(1, item["stype"]);
	QList<QNetworkInterface> netList=QNetworkInterface::allInterfaces();
	query.bindValue(2, netList.at(0).hardwareAddress());

	query.exec();
	if (QSqlError::NoError != query.lastError().type()) {
		return nRet;
	}

	nRet=query.lastInsertId().toLongLong();

	return nRet;
}

bool CDBSyncStore::updateSync(const QVariantMap &item) {
	// sync type
	// "id":id
	// "sid": server id
	// "sdate": sync time
	// "type": sync type
	bool bRet=false;
	QString szCond[] = {
		"id",
		""
	};

	QString szField[] = {
		"value",
		""
	};

	QSqlQuery query=m_db.exec();

	query.exec("begin;");
	int nCondIndex=0;
	while (!szCond[nCondIndex].isEmpty()) {
		if (item.find(szCond[nCondIndex]) == item.end()) {
			nCondIndex++;
			continue;
		}
		int nIndex=0;
		while (!szField[nIndex].isEmpty()) {
			if (item.find(szField[nIndex]) == item.end()) {
				nIndex++;
				continue;
			}

			QString szCmd;
			szCmd.sprintf("update sync_info set %s=? "
						  "where %s=?", QSZ(szField[nIndex]), QSZ(szCond[nCondIndex]));

			query.prepare(szCmd);
			query.bindValue(0, item[szField[nIndex]]);
			query.bindValue(1, item[szCond[nCondIndex]]);
			query.exec();
			nIndex++;
		}
		nCondIndex++;
	}

	query.exec("commit;");

	return bRet;
}

