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
	QThread::msleep(2500);
}

void CTestGroup::testCLuaCore_cvCaptureScreen() {
						//"cv.cap_screen(0,0,400,200)\n"
	g_lua.run_as_string("local cv=opencv()\n"
						"x,y=cv.match(nil,\"d:/project/luatest/pattern.bmp\",0,0,600,400)\n"
						"cv.rect(x,y,50,50)\n"
						"cv.show_screen()\n");
	QThread::msleep(100);
}

void CTestGroup::testCLuaCore_syntaxLoadDll() {
	QThread::msleep(100);
}

#endif
