#include <QTest>
#include "luacore.h"
#include <QList>
#include "debug.h"
#include "testgroup.h"

void CTestGroup::testCLuaCore_runAsScript() {
	CLuaCore lua;
	lua.open("test.lua");
	lua.run_as_string("print(\"123\")");
	QString str=lua.getResult();
	lua.close();
	QCOMPARE(str, QString("123"));
}
