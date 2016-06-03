#include <QCoreApplication>
#include <QTest>
#include "testgroup.h"

#include "netio.h"

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	CTestGroup obj;
	return QTest::qExec(&obj, argc, argv);
}

