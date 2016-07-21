#include "csqlitestore.h"
#include "debug.h"
#include <QFile>
#include <QSqlError>
#include <QSqlRecord>

CSQLiteStore::CSQLiteStore() : CBaseStore(),
	m_szFile()
{
	m_db = QSqlDatabase::addDatabase("QSQLITE");
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
	m_mutex.unlock();
	return nRet;
}

bool CSQLiteStore::update(const QVariant &item) {
	m_mutex.lock();
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
	m_mutex.unlock();
	return false;
}

void CSQLiteStore::remove(const QVariant &item) {
	m_mutex.lock();
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
	m_mutex.unlock();
}

bool CSQLiteStore::query(std::list<QVariant> &result, char *fmt, ...) {
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

void CSQLiteStore::insertItemMap(QVariantMap &item, QSqlRecord &rinfo, QSqlQuery &query, int nIndex, int nCount) {
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

bool CSQLiteStore::query(QSqlQuery &q, char *fmt, ...) {
	q=m_db.exec();

	va_list vlist;
	va_start (vlist, fmt);
	char szBuf[1024]={0};
	vsprintf(szBuf, fmt, vlist);
	va_end(vlist);
	return q.exec(szBuf);
}

void CSQLiteStore::close() {
	m_db.close();
	QSqlDatabase::removeDatabase("QSQLITE");
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
		"on target (mac);"
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
		"mode integer, "
		"idate integer);"
		);

	if (QSqlError::NoError != m_db.lastError().type()) {
		return false;
	}

	// create index in board_info
	m_db.exec(
		"create index idx_binfo_tid "
		"on board_info (tid);"
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
		"tdate integer);"
		);

	if (QSqlError::NoError != m_db.lastError().type()) {
		return false;
	}

	// create index in item_info
	m_db.exec(
		"create index idx_iinfo_tid "
		"on item_info (tid);"
		);

	if (QSqlError::NoError != m_db.lastError().type()) {
		return false;
	}

	m_db.exec(
		"create index idx_iinfo_bid "
		"on item_info (bid);"
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
	// "sdate": start date
	// "id": id
	long long nRet=0;
	QSqlQuery query=m_db.exec();

	query.prepare("insert into target (mac,wonum,board_name,sdate) "
				  "values (?,?,?,?);"
				  );

	query.bindValue(0, item["mac"]);
	query.bindValue(1, item["wonum"]);
	query.bindValue(2, item["board_name"]);
	query.bindValue(3, item["sdate"]);

	query.exec();
	if (QSqlError::NoError != query.lastError().type()) {
		return nRet;
	}

	nRet=query.lastInsertId().toLongLong();

	return nRet;
}

long long CSQLiteStore::addBoard(const QVariantMap &item) {
	// board type
	// "tid": target id
	// "id": id
	// "result": final test result
	long long nRet=0;

	QSqlQuery query=m_db.exec();

	query.prepare("insert into board_info (tid,mode) "
				  "values (?,?);"
				  );

	query.bindValue(0, item["tid"]);
#ifdef DEBUG
	query.bindValue(1, 1);
#else
	query.bindValue(1, 0);
#endif

	query.exec();
	if (QSqlError::NoError != query.lastError().type()) {
		return nRet;
	}

	nRet=query.lastInsertId().toLongLong();

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

	QSqlQuery query=m_db.exec();

	if (item.find("result") == item.end()) {
		query.prepare("insert into item_info (tid,bid,name,tdate) "
					  "values (?,?,?,?);"
					  );
		query.bindValue(0, item["tid"]);
		query.bindValue(1, item["bid"]);
		query.bindValue(2, item["name"]);
		query.bindValue(3, item["tdate"]);
	}
	else {
		query.prepare("insert into item_info (tid,bid,name,tdate,result) "
					  "values (?,?,?,?,?);"
					  );
		query.bindValue(0, item["tid"]);
		query.bindValue(1, item["bid"]);
		query.bindValue(2, item["name"]);
		query.bindValue(3, item["tdate"]);
		query.bindValue(4, item["result"]);
	}

	query.exec();
	if (QSqlError::NoError != query.lastError().type()) {
		return nRet;
	}

	nRet=query.lastInsertId().toLongLong();

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

	QSqlQuery query=m_db.exec();

	QString szCond[] = {
		"mac",
		"id",
		""
	};

	QString szField[] = {
		"unum",
		"board_name",
		"wonum",
		""
	};

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
			szCmd.sprintf("update target set %s=? "
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
	bRet=true;

	return bRet;
}

bool CSQLiteStore::updateBoard(const QVariantMap &item) {
	// board type
	// "tid": target id
	// "id": id
	// "result": final test result
	//
	bool bRet=false;
	QSqlQuery query=m_db.exec();

	QString szCond[] = {
		"id",
		""
	};

	QString szField[] = {
		"tid",
		"result",
		""
	};

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
			szCmd.sprintf("update board_info set %s=? "
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
	bRet=true;

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
	QSqlQuery query=m_db.exec();

	QString szCond[] = {
		"id",
		""
	};

	QString szField[] = {
		"tid",
		"bid",
		"name",
		"result",
		"tdate",
		""
	};

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
			szCmd.sprintf("update item_info set %s=? "
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

void CSQLiteStore::removeTarget(const QVariantMap &item) {
	// target type
	// "mac": mac address
	// "wonum": work number
	// "unum": unit output number
	// "board_name": target board full name
	// "idate": start date
	// "id": id

	QSqlQuery query=m_db.exec();
	query.exec("begin;");
	QString szCond[] = {
		"wonum",
		"mac",
		"id"
	};

	int nCondIndex=0;
	while (!szCond[nCondIndex].isEmpty()) {
		if (item.find(szCond[nCondIndex]) == item.end()) {
			nCondIndex++;
			continue;
		}

		std::list<QVariant> lst;
		if (!this->query(lst, "select id from target where %s=%s;",
				   QSZ(szCond[nCondIndex]), QSZ(item[szCond[nCondIndex]].toString()))) {
			nCondIndex++;
			continue;
		}

		if (0 >= lst.size()) {
			nCondIndex++;
			continue;
		}

		long long id=lst.begin()->toMap()["id"].toLongLong();

		QString szCmd;
		szCmd.sprintf("delete from item_info where tid=%lld;", id);
		query.exec(szCmd);

		szCmd.sprintf("delete from board_info where tid=%lld;", id);
		query.exec(szCmd);

		szCmd.sprintf("delete from target where id=%lld;", id);
		query.exec(szCmd);

		nCondIndex++;
		break;
	}
	query.exec("commit;");

}

void CSQLiteStore::removeBoard(const QVariantMap &item) {
	// board type
	// "tid": target id
	// "id": id
	// "result": final test result

	QSqlQuery query=m_db.exec();
	query.exec("begin;");
	QString szCond[] = {
		"id"
	};

	int nCondIndex=0;
	while (!szCond[nCondIndex].isEmpty()) {
		if (item.find(szCond[nCondIndex]) == item.end()) {
			nCondIndex++;
			continue;
		}

		std::list<QVariant> lst;
		if (!this->query(lst, "select id from board_info where %s=%s;",
						 QSZ(szCond[nCondIndex]), QSZ(item[szCond[nCondIndex]].toString()))) {
			nCondIndex++;
			continue;
		}

		if (0 >= lst.size()) {
			nCondIndex++;
			continue;
		}

		long long id=lst.begin()->toMap()["id"].toLongLong();

		QString szCmd;
		szCmd.sprintf("delete from item_info where bid=%lld;", id);
		query.exec(szCmd);

		szCmd.sprintf("delete from board_info where id=%lld;", id);
		query.exec(szCmd);

		nCondIndex++;
		break;
	}
	query.exec("commit;");

}

void CSQLiteStore::removeItem(const QVariantMap &item) {
	// item type
	// "id":id
	// "tid": target id
	// "bid": board id
	// "name": item name
	// "result": test result
	// "tdate": test date

	QSqlQuery query=m_db.exec();
	query.exec("begin;");
	QString szCond[] = {
		"name",
		"id"
	};

	int nCondIndex=0;
	while (!szCond[nCondIndex].isEmpty()) {
		if (item.find(szCond[nCondIndex]) == item.end()) {
			nCondIndex++;
			continue;
		}

		std::list<QVariant> lst;
		if (!this->query(lst, "select id from item_info where %s=%s;",
						 QSZ(szCond[nCondIndex]), QSZ(item[szCond[nCondIndex]].toString()))) {
			nCondIndex++;
			continue;
		}

		if (0 >= lst.size()) {
			nCondIndex++;
			continue;
		}

		long long id=lst.begin()->toMap()["id"].toLongLong();

		QString szCmd;
		szCmd.sprintf("delete from item_info where id=%lld;", id);
		query.exec(szCmd);

		nCondIndex++;
		break;
	}
	query.exec("commit;");


}
