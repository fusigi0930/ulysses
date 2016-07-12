#include <QTest>
#include "csqlitestore.h"
#include <QList>
#include "debug.h"
#include "testgroup.h"
#include <QThread>
#include <QSignalSpy>

void CTestGroup::testCSQLiteStore_initDB() {
	CSQLiteStore db;
	db.open("D:\\test.db");

	db.close();
	QCOMPARE(QFile::exists("D:\\test.db"), true);
}
