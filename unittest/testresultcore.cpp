#include <QTest>
#include "csqlitestore.h"
#include <QList>
#include "debug.h"
#include "testgroup.h"
#include <QThread>
#include <QSignalSpy>
#include <list>

#define TEST_DB "D:\\test.db"

void CTestGroup::testCSQLiteStore_initDB() {
	CSQLiteStore db;
	db.open(TEST_DB);

	db.close();
	QCOMPARE(QFile::exists(TEST_DB), true);

	db.open(TEST_DB);
	std::list<QVariant> lst;

	db.query(lst, "select * from sqlite_master;");
	std::list<QVariant>::iterator pItem;

	bool bResult=false;
	for (pItem=lst.begin(); pItem != lst.end(); pItem++) {
		QVariantMap mapItem=pItem->toMap();
		bResult=mapItem["sql"].toString().contains("create table item_info", Qt::CaseInsensitive);
		if (bResult) break;
	}

	QFile::remove(TEST_DB);
	QCOMPARE(bResult, true);
}

void CTestGroup::testCSQLiteStore_addTarget() {
	// target type
	// "mac": mac address
	// "wonum": work number
	// "unum": unit output number
	// "board_name": target board full name
	// "sdate": start date
	// "id": id

	CSQLiteStore db;
	db.open(TEST_DB);

	QVariantMap item;
	item.insert("type", "target");
	item.insert("mac", "11:22:33:44:55:66");
	item.insert("wonum", "WO-123456789012");
	item.insert("unum", "qazxsw");
	item.insert("board_name", "ABCDEFG");
	item.insert("sdate", QDateTime::currentMSecsSinceEpoch());

	long long id=db.add(QVariant::fromValue(item));
	QCOMPARE(id, 1);

	std::list<QVariant> lst;
	db.query(lst, "select count(*) from target;");
	QCOMPARE(lst.begin()->toMap().begin().value().toLongLong(), 1);

	item.clear();
	item.insert("type", "target");
	item.insert("mac", "12:32:33:44:55:66");
	item.insert("wonum", "WO-223456789012");
	item.insert("unum", "qazxsw");
	item.insert("board_name", "2ABCDEFG");
	item.insert("sdate", QDateTime::currentMSecsSinceEpoch());

	id=db.add(QVariant::fromValue(item));
	QCOMPARE(id, 2);
	lst.clear();
	db.query(lst, "select * from target;");
	QCOMPARE(lst.size(), static_cast<unsigned int>(2));

	std::list<QVariant>::iterator pItem=lst.begin();
	QCOMPARE(pItem->toMap()["id"].toLongLong(), 1);
	QCOMPARE(pItem->toMap()["mac"].toString().compare("11:22:33:44:55:66"), 0);
	pItem++;

	QCOMPARE(pItem->toMap()["id"].toLongLong(), 2);
	QCOMPARE(pItem->toMap()["mac"].toString().compare("12:32:33:44:55:66"), 0);
	db.close();
}

void CTestGroup::testCSQLiteStore_addBoard() {
	// board type
	// "tid": target id
	// "id": id
	// "result": final test result
	CSQLiteStore db;
	db.open(TEST_DB);

	QVariantMap item;
	item.insert("type", "board");
	item.insert("tid", 1);
	item.insert("result", 1);


	long long id=db.add(QVariant::fromValue(item));
	QCOMPARE(id, 1);

	std::list<QVariant> lst;
	db.query(lst, "select count(*) from board_info;");
	QCOMPARE(lst.begin()->toMap().begin().value().toLongLong(), 1);

	item.clear();
	item.insert("type", "board");
	item.insert("tid", 1);
	item.insert("result", 0);

	id=db.add(QVariant::fromValue(item));
	QCOMPARE(id, 2);

	item.clear();
	item.insert("type", "board");
	item.insert("tid", 2);
	item.insert("result", 0);

	id=db.add(QVariant::fromValue(item));
	QCOMPARE(id, 3);

	lst.clear();
	db.query(lst, "select * from board_info;");
	QCOMPARE(lst.size(), static_cast<unsigned int>(3));

	std::list<QVariant>::iterator pItem=lst.begin();
	QCOMPARE(pItem->toMap()["id"].toLongLong(), 1);
	QCOMPARE(pItem->toMap()["tid"].toLongLong(), 1);
	pItem++;

	QCOMPARE(pItem->toMap()["id"].toLongLong(), 2);
	QCOMPARE(pItem->toMap()["tid"].toLongLong(), 1);
	pItem++;

	QCOMPARE(pItem->toMap()["id"].toLongLong(), 3);
	QCOMPARE(pItem->toMap()["tid"].toLongLong(), 2);

	db.close();
}

void CTestGroup::testCSQLiteStore_addItem() {
	// item type
	// "id":id
	// "tid": target id
	// "bid": board id
	// "name": item name
	// "result": test result
	// "tdate": test date

	CSQLiteStore db;
	db.open(TEST_DB);

	QVariantMap item;
	item.insert("type", "item");
	item.insert("tid", 1);
	item.insert("bid", 1);
	item.insert("name", "abc");
	item.insert("result", 0);
	item.insert("tdate", QDateTime::currentMSecsSinceEpoch());


	long long id=db.add(QVariant::fromValue(item));
	QCOMPARE(id, 1);

	std::list<QVariant> lst;

	item.clear();
	item.insert("type", "item");
	item.insert("tid", 1);
	item.insert("bid", 1);
	item.insert("name", "ddd");
	item.insert("result", 0);
	item.insert("tdate", QDateTime::currentMSecsSinceEpoch());

	id=db.add(QVariant::fromValue(item));
	QCOMPARE(id, 2);

	item.clear();
	item.insert("type", "item");
	item.insert("tid", 2);
	item.insert("bid", 2);
	item.insert("name", "eee");
	item.insert("result", 0);
	item.insert("tdate", QDateTime::currentMSecsSinceEpoch());

	id=db.add(QVariant::fromValue(item));
	QCOMPARE(id, 3);

	item.clear();
	item.insert("type", "item");
	item.insert("tid", 2);
	item.insert("bid", 3);
	item.insert("name", "fff");
	item.insert("result", 0);
	item.insert("tdate", QDateTime::currentMSecsSinceEpoch());

	id=db.add(QVariant::fromValue(item));
	QCOMPARE(id, 4);

	lst.clear();
	db.query(lst, "select * from item_info;");
	QCOMPARE(lst.size(), static_cast<unsigned int>(4));

	std::list<QVariant>::iterator pItem=lst.begin();
	QCOMPARE(pItem->toMap()["id"].toLongLong(), 1);
	QCOMPARE(pItem->toMap()["tid"].toLongLong(), 1);
	QCOMPARE(pItem->toMap()["bid"].toLongLong(), 1);
	QCOMPARE(pItem->toMap()["name"].toString().compare("abc"), 0);
	pItem++;

	QCOMPARE(pItem->toMap()["id"].toLongLong(), 2);
	QCOMPARE(pItem->toMap()["tid"].toLongLong(), 1);
	QCOMPARE(pItem->toMap()["bid"].toLongLong(), 1);
	QCOMPARE(pItem->toMap()["name"].toString().compare("ddd"), 0);
	pItem++;

	QCOMPARE(pItem->toMap()["id"].toLongLong(), 3);
	QCOMPARE(pItem->toMap()["tid"].toLongLong(), 2);
	QCOMPARE(pItem->toMap()["bid"].toLongLong(), 2);
	QCOMPARE(pItem->toMap()["name"].toString().compare("eee"), 0);
	pItem++;

	QCOMPARE(pItem->toMap()["id"].toLongLong(), 4);
	QCOMPARE(pItem->toMap()["tid"].toLongLong(), 2);
	QCOMPARE(pItem->toMap()["bid"].toLongLong(), 3);
	QCOMPARE(pItem->toMap()["name"].toString().compare("fff"), 0);

	db.close();

}

void CTestGroup::testCSQLiteStore_updateTarget() {
	CSQLiteStore db;
	db.open(TEST_DB);

	QVariantMap item;
	std::list<QVariant> lst;
	db.query(lst, "select * from target where id=1;");
	QCOMPARE(lst.begin()->toMap()["wonum"].toString().compare("WO-123456789012"), 0);

	item.insert("type", "target");
	item.insert("id", 1);
	item.insert("wonum", "WO-987654321098");
	db.update(item);
	lst.clear();

	db.query(lst, "select * from target where id=1;");
	QCOMPARE(lst.begin()->toMap()["wonum"].toString().compare("WO-987654321098"), 0);
	db.close();

}

void CTestGroup::testCSQLiteStore_updateBoard() {
	CSQLiteStore db;
	db.open(TEST_DB);

	QVariantMap item;
	std::list<QVariant> lst;
	db.query(lst, "select * from board_info where id=1;");
	QCOMPARE(lst.begin()->toMap()["tid"].toLongLong(), 1);

	item.insert("type", "board");
	item.insert("id", 1);
	item.insert("tid", 99);
	db.update(item);
	lst.clear();

	db.query(lst, "select * from board_info where id=1;");
	QCOMPARE(lst.begin()->toMap()["tid"].toLongLong(), 99);
	db.close();
}

void CTestGroup::testCSQLiteStore_updateItem() {
	CSQLiteStore db;
	db.open(TEST_DB);

	QVariantMap item;
	std::list<QVariant> lst;
	db.query(lst, "select * from item_info where id=1;");
	QCOMPARE(lst.begin()->toMap()["name"].toString().compare("abc"), 0);

	item.insert("type", "item");
	item.insert("id", 1);
	item.insert("name", "doraemon");
	db.update(item);
	lst.clear();

	db.query(lst, "select * from item_info where id=1;");
	QCOMPARE(lst.begin()->toMap()["name"].toString().compare("doraemon"), 0);
	db.close();
}

void CTestGroup::testCSQLiteStore_removeTarget() {
	CSQLiteStore db;
	db.open(TEST_DB);

	QVariantMap item;
	std::list<QVariant> lst;
	db.query(lst, "select count(*) from target where id=1;");
	QCOMPARE(lst.begin()->toMap().begin()->toLongLong(), 1);

	db.query(lst, "select count(*) from board_info where tid=1;");
	QCOMPARE(lst.begin()->toMap().begin()->toLongLong() > 0, true);

	db.query(lst, "select count(*) from item_info where tid=1;");
	QCOMPARE(lst.begin()->toMap().begin()->toLongLong() > 0, true);

	item.insert("type", "target");
	item.insert("id", 1);

	db.remove(QVariant::fromValue(item));

	db.query(lst, "select count(*) from target where id=1;");
	QCOMPARE(lst.begin()->toMap().begin()->toLongLong(), 0);

	db.query(lst, "select count(*) from board_info where tid=1;");
	QCOMPARE(lst.begin()->toMap().begin()->toLongLong(), 0);

	db.query(lst, "select count(*) from item_info where tid=1;");
	QCOMPARE(lst.begin()->toMap().begin()->toLongLong(), 0);

	db.close();
}

void CTestGroup::testCSQLiteStore_removeBoard() {
	CSQLiteStore db;
	db.open(TEST_DB);

	QVariantMap item;
	std::list<QVariant> lst;
	db.query(lst, "select count(*) from board_info where id=3;");
	QCOMPARE(lst.begin()->toMap().begin()->toLongLong(), 1);

	db.query(lst, "select count(*) from item_info where bid=3;");
	QCOMPARE(lst.begin()->toMap().begin()->toLongLong(), 1);

	item.insert("type", "board");
	item.insert("id", 3);

	db.remove(QVariant::fromValue(item));

	db.query(lst, "select count(*) from board_info where id=3;");
	QCOMPARE(lst.begin()->toMap().begin()->toLongLong(), 0);

	db.query(lst, "select count(*) from item_info where bid=3;");
	QCOMPARE(lst.begin()->toMap().begin()->toLongLong(), 0);

	db.close();
}

void CTestGroup::testCSQLiteStore_removeItem() {
	CSQLiteStore db;
	db.open(TEST_DB);

	QVariantMap item;
	std::list<QVariant> lst;
	db.query(lst, "select count(*) from item_info where id=3;");
	QCOMPARE(lst.begin()->toMap().begin()->toLongLong(), 1);

	item.insert("type", "item");
	item.insert("id", 3);

	db.remove(QVariant::fromValue(item));

	db.query(lst, "select count(*) from item_info where id=3;");
	QCOMPARE(lst.begin()->toMap().begin()->toLongLong(), 0);

	db.close();
	QFile::remove(TEST_DB);
}

void CTestGroup::testCDBSyncStore_initDB() {
	CDBSyncStore syncStore;
	QCOMPARE(syncStore.open(NULL), true);

	std::list<QVariant> lst;
	syncStore.query(lst, "select * from target where id=1;");

	QCOMPARE((lst.size() != 0), true);

	QCOMPARE(lst.begin()->toMap()["mac"].toString().compare("F80278810055"), 0);
}
