#ifndef __TESTGROUP_H__
#define __TESTGROUP_H__

#include <QObject>

class CTestGroup : public QObject {
	Q_OBJECT

public:
	CTestGroup(QObject *parent=NULL) : QObject(parent) {

	}

	virtual ~CTestGroup() {

	}

private slots:
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

	void testCLuaCore_runAsScript();

#if 0
	void testCNetcatIO_openServer();
	void testCNotifyRecv_broadcastProcess();
	void testCTelnetIO_openClient();
#endif

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
};

#endif // __TESTGROUP_H__

