#include <QTest>
#include "xmlrun.h"
#include "runitem.h"
#include <QList>
#include "testgroup.h"

void CTestGroup::slotCXmlRun_saveFile() {
	CXmlRun xmlrun;
	SItem item1;
	item1.szName="item1";
	item1.szDesc="test item 1";
	item1.szPreProc="preProc";
	item1.szProc="Proc";
	item1.szPostProc="postProc";
	item1.szExpect="test expect";
	item1.nDelay=100;
	item1.nCmdType=0;

	xmlrun.getItems().push_back(item1);
	if (!xmlrun.save("test.xml")) {
		QFAIL("save file failed!");
	}

	QFile file;
	file.setFileName("test.xml");
	if (!file.open(QFile::ReadOnly)) {
		QFAIL("open file failed!");
	}
	QString szContent=file.readAll().toStdString().c_str();
	file.close();
	QCOMPARE(szContent, QString("<?xml version=\"1.0\" encoding=\"UTF-8\"?><testcase><case><item>item1</item><comment>test item 1</comment><precmd>preProc</precmd><command>Proc</command><postcmd>postProc</postcmd><delay>100</delay><expect>test expect</expect><type>auto</type></case></testcase>\n"));
}

void CTestGroup::slotCXmlRun_openFile() {
	CXmlRun xmlrun;
	if (!xmlrun.open("test.xml")) {
		QFAIL("open test file failed!");
	}

	QCOMPARE(xmlrun.getItemCount(), 1);
	xmlrun.setCurrentIndex(0);
	SItem item=xmlrun.getCurrentItem();

	QCOMPARE(item.szName, QString("item1"));
	QCOMPARE(item.szDesc, QString("test item 1"));
	QCOMPARE(item.szPreProc, QString("preProc"));
	QCOMPARE(item.szProc, QString("Proc"));
	QCOMPARE(item.szPostProc, QString("postProc"));
	QCOMPARE(item.szExpect, QString("test expect"));
	QCOMPARE(static_cast<int>(item.nDelay), 100);
	QCOMPARE(static_cast<int>(item.nCmdType), 0);
	return;
}

void CTestGroup::slotCXmlRun_replaceItem() {
	CXmlRun xmlrun;
	if (!xmlrun.open("test.xml")) {
		QFAIL("open test file failed!");
	}

	SItem &item=xmlrun.getItem(0);
	item.szName="replace1";

	if (!xmlrun.save("replace.xml")) {
		QFAIL("save file failed!");
	}
	CXmlRun xmlrun1;
	if (!xmlrun1.open("replace.xml")) {
		QFAIL("open test file failed!");
	}
	SItem item1=xmlrun1.getCurrentItem();
	QCOMPARE(item1.szName, QString("replace1"));
}

void CTestGroup::slotCXmlRun_escapeCharSave() {
	CXmlRun xmlrun;
	SItem item1;
	item1.szName="escape";
	item1.szDesc="&><\"'";
	item1.szPreProc="";
	item1.szProc="Proc";
	item1.szPostProc="postProc";
	item1.szExpect="test expect";
	item1.nDelay=100;
	item1.nCmdType=0;

	xmlrun.getItems().push_back(item1);
	if (!xmlrun.save("escape.xml")) {
		QFAIL("save file failed!");
	}

	CXmlRun xmlrun1;
	if (!xmlrun1.open("escape.xml")) {
		QFAIL("open test file failed!");
	}

	QCOMPARE(xmlrun1.getCurrentItem().szName, QString("&><\"'"));
}

void CTestGroup::slotCXmlRun_Utf8CharSave() {

}

void CTestGroup::slotCXmlRun_Utf8CharRead() {

}
