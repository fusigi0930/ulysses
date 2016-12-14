#include <QTest>
#include "luacore.h"
#include <QList>
#include "debug.h"
#include "testgroup.h"

#if TESTITEM & TEST_LUA
CLuaCore g_lua;

void CTestGroup::testCLuaCore_runAsScript() {

	g_lua.run_as_string("print(\"123\")");
	QString str=g_lua.getResult();

	QCOMPARE(str, QString("123"));
}

void CTestGroup::testCLuaCore_open() {
	g_lua.open("d:\\project\\luatest\\open_test.lua");
	QThread::msleep(1000);

	QString str=g_lua.getResult();
	QCOMPARE(str, QString("open test file"));
}

void CTestGroup::testCLuaCore_runThread() {
	g_lua.create("d:\\project\\luatest\\run_thread.lua");
	QThread::msleep(1000);

	QString str=g_lua.getResult();
	QCOMPARE(str, QString("run thread"));
}

void CTestGroup::testCLuaCore_syntaxRequire() {
	g_lua.create("d:\\project\\luatest\\syntax_require.lua");
	QThread::msleep(1000);

	QString str=g_lua.getResult();
	QCOMPARE(str, QString("require: 50"));
}

void CTestGroup::testCLuaCore_syntaxRequireVariable() {
	g_lua.create("d:\\project\\luatest\\syntax_require_const.lua");
	QThread::msleep(1000);

	QString str=g_lua.getResult();
	QCOMPARE(str, QString("external variable"));
}

void CTestGroup::testCLuaCore_defaultConst() {
	g_lua.create("d:\\project\\luatest\\default_const.lua");
	QThread::msleep(1000);

	QString str=g_lua.getResult();
	QCOMPARE(str, QString("vk_a: 65"));
}

void CTestGroup::testCLuaCore_sendEventKey() {
	g_lua.create("d:\\project\\luatest\\sendevent_key.lua");
	QThread::msleep(1000);

	QCOMPARE(1,1);
}


#endif
