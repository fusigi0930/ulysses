#ifndef __TESTGROUP_H__
#define __TESTGROUP_H__

#include <QObject>

#define TEST_XML		(1 << 0)
#define TEST_NETIO		(1 << 1)
#define TEST_SQL		(1 << 2)
#define TEST_LUA		(1 << 3)

#define TESTITEM		(TEST_LUA)

class CTestGroup : public QObject {
	Q_OBJECT

public:
	CTestGroup(QObject *parent=NULL) : QObject(parent) {

	}

	virtual ~CTestGroup() {

	}

private slots:
#if TESTITEM & TEST_XML
	void testCXmlRun_saveFile();
	void testCXmlRun_openFile();
	void testCXmlRun_replaceItem();
	void testCXmlRun_escapeCharSave();
	void testCXmlRun_Utf8CharSave();
	void testCXmlRun_Utf8CharRead();
	void testCXmlRun_multipleItem();

	void testCXmlFactoryRun_saveFile();
	void testCXmlFactoryRun_openFile();
	void testCXmlFactoryRun_replaceItem();
	void testCXmlFactoryRun_escapeCharSave();
	void testCXmlFactoryRun_Utf8CharSave();
	void testCXmlFactoryRun_Utf8CharRead();
	void testCXmlFactoryRun_multipleItem();

	void testBaseXml();
#endif

#if TESTITEM & TEST_LUA
	void testCLuaCore_runAsScript();
	void testCLuaCore_open();
	void testCLuaCore_runThread();
	void testCLuaCore_syntaxRequire();
	void testCLuaCore_syntaxRequireVariable();
	void testCLuaCore_defaultConst();
	void testCLuaCore_sendEventKey();
	void testCLuaCore_cvCaptureScreen();
	void testCLuaCore_syntaxLoadDll();
#endif

#if TESTITEM & TEST_NETIO
	void testCNetcatIO_openServer();
	void testCNotifyRecv_broadcastProcess();
	void testCTelnetIO_openClient();
#endif

#if TESTITEM & TEST_SQL
	void testCSQLiteStore_initDB();
	void testCSQLiteStore_addTarget();
	void testCSQLiteStore_addBoard();
	void testCSQLiteStore_addItem();
	void testCSQLiteStore_updateTarget();
	void testCSQLiteStore_updateBoard();
	void testCSQLiteStore_updateItem();
	void testCSQLiteStore_removeTarget();
	void testCSQLiteStore_removeBoard();
	void testCSQLiteStore_removeItem();

	void testCDBSyncStore_initDB();
	void testDBSyncInfo();
#endif
};

#endif // __TESTGROUP_H__

